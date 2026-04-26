/* =============================================================================
 * nanochrono.c  –  High-level C API  (unified implementation)
 *
 * Compilation modes (controlled by preprocessor defines):
 *
 *   NC_LEGACY_BUILD      → compile-time lock to CPUID backend (legacy DLL)
 *   NC_AVX_BUILD         → compile-time lock to LFENCE/VZEROALL backend (AVX DLL)
 *   NC_UNIFIED_BUILD     → runtime dispatch: picks best backend via CPUID+XSAVE
 *   (none of the above)  → same as NC_UNIFIED_BUILD
 *
 * The DLLs use NC_LEGACY_BUILD / NC_AVX_BUILD so the call is inlined with no
 * branch.  The standalone exe uses NC_UNIFIED_BUILD so BOTH ASM object files
 * can be linked together and the right one is chosen at nc_create() time.
 *
 * All arithmetic on (cycles * 1e9 / hz) uses _umul128 / _udiv128 to avoid
 * overflow on long runtimes.
 * ============================================================================= */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <intrin.h>
#include <stdlib.h>
#include <string.h>

/* Export/import decoration ───────────────────────────────────────────────── */
#ifdef NANOCHRONO_EXPORTS
#  define NC_API __declspec(dllexport)
#elif defined(NANOCHRONO_DLL)
#  define NC_API __declspec(dllimport)
#else
#  define NC_API   /* static / unified build */
#endif

#include "nanochrono.h"


/* ISA-isolated ASM backends. These are referenced only in unified builds. */
#if !defined(NC_LEGACY_BUILD) && !defined(NC_AVX_BUILD)
extern uint64_t nc_mmx_tsc_start(void);       extern uint64_t nc_mmx_tsc_end(void);       extern uint64_t nc_mmx_tsc_overhead(void);
extern uint64_t nc_sse_tsc_start(void);       extern uint64_t nc_sse_tsc_end(void);       extern uint64_t nc_sse_tsc_overhead(void);
extern uint64_t nc_sse2_tsc_start(void);      extern uint64_t nc_sse2_tsc_end(void);      extern uint64_t nc_sse2_tsc_overhead(void);
extern uint64_t nc_sse3_tsc_start(void);      extern uint64_t nc_sse3_tsc_end(void);      extern uint64_t nc_sse3_tsc_overhead(void);
extern uint64_t nc_ssse3_tsc_start(void);     extern uint64_t nc_ssse3_tsc_end(void);     extern uint64_t nc_ssse3_tsc_overhead(void);
extern uint64_t nc_sse41_tsc_start(void);     extern uint64_t nc_sse41_tsc_end(void);     extern uint64_t nc_sse41_tsc_overhead(void);
extern uint64_t nc_sse42_tsc_start(void);     extern uint64_t nc_sse42_tsc_end(void);     extern uint64_t nc_sse42_tsc_overhead(void);
extern uint64_t nc_f16c_tsc_start(void);      extern uint64_t nc_f16c_tsc_end(void);      extern uint64_t nc_f16c_tsc_overhead(void);
extern uint64_t nc_fma_tsc_start(void);       extern uint64_t nc_fma_tsc_end(void);       extern uint64_t nc_fma_tsc_overhead(void);
extern uint64_t nc_avx2_tsc_start(void);      extern uint64_t nc_avx2_tsc_end(void);      extern uint64_t nc_avx2_tsc_overhead(void);
extern uint64_t nc_avx_vnni_tsc_start(void);  extern uint64_t nc_avx_vnni_tsc_end(void);  extern uint64_t nc_avx_vnni_tsc_overhead(void);
extern uint64_t nc_avx512_tsc_start(void);    extern uint64_t nc_avx512_tsc_end(void);    extern uint64_t nc_avx512_tsc_overhead(void);
extern uint64_t nc_avx512_vnni_tsc_start(void); extern uint64_t nc_avx512_vnni_tsc_end(void); extern uint64_t nc_avx512_vnni_tsc_overhead(void);
#endif

/* ─────────────────────────────────────────────────────────────────────────────
 * QPC helpers
 * ───────────────────────────────────────────────────────────────────────────── */
static __forceinline uint64_t _qpc_now(void) {
    LARGE_INTEGER t; QueryPerformanceCounter(&t);
    return (uint64_t)t.QuadPart;
}
static __forceinline uint64_t _qpc_freq(void) {
    LARGE_INTEGER f; QueryPerformanceFrequency(&f);
    return (uint64_t)f.QuadPart;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * 128-bit safe arithmetic helpers
 * ───────────────────────────────────────────────────────────────────────────── */
static __forceinline uint64_t _mul_div(uint64_t v, uint64_t mul, uint64_t div) {
    if (!v || !mul || !div) return 0;
    unsigned __int64 hi, rem;
    unsigned __int64 lo = _umul128(v, mul, &hi);
    return (uint64_t)_udiv128(hi, lo, (unsigned __int64)div, &rem);
}

static __forceinline uint64_t _cycles_to_ns(uint64_t c, uint64_t hz)
    { return _mul_div(c, 1000000000ULL, hz); }
static __forceinline uint64_t _cycles_to_us(uint64_t c, uint64_t hz)
    { return _mul_div(c, 1000000ULL, hz); }
static __forceinline uint64_t _cycles_to_ms(uint64_t c, uint64_t hz)
    { return _mul_div(c, 1000ULL, hz); }
static __forceinline uint64_t _ns_to_cycles(uint64_t ns, uint64_t hz)
    { return _mul_div(ns, hz, 1000000000ULL); }
static __forceinline uint64_t _hz_from_window(uint64_t dc, uint64_t dq, uint64_t qhz)
    { return dq ? _mul_div(dc, qhz, dq) : 0; }

/* ─────────────────────────────────────────────────────────────────────────────
 * Backend dispatch
 *
 *  NC_LEGACY_BUILD  → always call nc_tsc_*   (CPUID serialised)
 *  NC_AVX_BUILD     → always call nc_avx_tsc_* (LFENCE)
 *  unified/none     → branch on ctx->backend at call site
 *
 * Using __forceinline + compile-time constants means the branch is eliminated
 * by the compiler in the DLL builds.
 * ───────────────────────────────────────────────────────────────────────────── */

#if defined(NC_LEGACY_BUILD)
#  define _BE_START(b)  nc_tsc_start()
#  define _BE_END(b)    nc_tsc_end()
#elif defined(NC_AVX_BUILD)
#  define _BE_START(b)  nc_avx_tsc_start()
#  define _BE_END(b)    nc_avx_tsc_end()
#else
/* NC_UNIFIED_BUILD or default: runtime dispatch.
 * Every ISA family has its own object file. The dispatcher reaches a family only
 * after nc_backend_is_available() passes CPUID/XGETBV, preventing illegal
 * instruction faults on older CPUs and on OSes that did not enable YMM/ZMM state.
 */
#  define _NC_BACKEND_IS_AVX_FAMILY(b) ((b)==NC_BACKEND_AVX || (b)==NC_BACKEND_F16C || (b)==NC_BACKEND_FMA || (b)==NC_BACKEND_AVX2 || (b)==NC_BACKEND_AVX_VNNI || (b)==NC_BACKEND_AVX512 || (b)==NC_BACKEND_AVX512VNNI)
static __forceinline uint64_t _be_start(nc_backend_t b) {
    switch (b) {
    case NC_BACKEND_MMX: return nc_mmx_tsc_start();
    case NC_BACKEND_SSE: return nc_sse_tsc_start();
    case NC_BACKEND_SSE2: return nc_sse2_tsc_start();
    case NC_BACKEND_SSE3: return nc_sse3_tsc_start();
    case NC_BACKEND_SSSE3: return nc_ssse3_tsc_start();
    case NC_BACKEND_SSE41: return nc_sse41_tsc_start();
    case NC_BACKEND_SSE42: return nc_sse42_tsc_start();
    case NC_BACKEND_AVX: return nc_avx_tsc_start();
    case NC_BACKEND_F16C: return nc_f16c_tsc_start();
    case NC_BACKEND_FMA: return nc_fma_tsc_start();
    case NC_BACKEND_AVX2: return nc_avx2_tsc_start();
    case NC_BACKEND_AVX_VNNI: return nc_avx_vnni_tsc_start();
    case NC_BACKEND_AVX512: return nc_avx512_tsc_start();
    case NC_BACKEND_AVX512VNNI: return nc_avx512_vnni_tsc_start();
    case NC_BACKEND_LEGACY:
    default: return nc_tsc_start();
    }
}
static __forceinline uint64_t _be_end(nc_backend_t b) {
    switch (b) {
    case NC_BACKEND_MMX: return nc_mmx_tsc_end();
    case NC_BACKEND_SSE: return nc_sse_tsc_end();
    case NC_BACKEND_SSE2: return nc_sse2_tsc_end();
    case NC_BACKEND_SSE3: return nc_sse3_tsc_end();
    case NC_BACKEND_SSSE3: return nc_ssse3_tsc_end();
    case NC_BACKEND_SSE41: return nc_sse41_tsc_end();
    case NC_BACKEND_SSE42: return nc_sse42_tsc_end();
    case NC_BACKEND_AVX: return nc_avx_tsc_end();
    case NC_BACKEND_F16C: return nc_f16c_tsc_end();
    case NC_BACKEND_FMA: return nc_fma_tsc_end();
    case NC_BACKEND_AVX2: return nc_avx2_tsc_end();
    case NC_BACKEND_AVX_VNNI: return nc_avx_vnni_tsc_end();
    case NC_BACKEND_AVX512: return nc_avx512_tsc_end();
    case NC_BACKEND_AVX512VNNI: return nc_avx512_vnni_tsc_end();
    case NC_BACKEND_LEGACY:
    default: return nc_tsc_end();
    }
}
static __forceinline uint64_t _be_overhead(nc_backend_t b) {
    switch (b) {
    case NC_BACKEND_MMX: return nc_mmx_tsc_overhead();
    case NC_BACKEND_SSE: return nc_sse_tsc_overhead();
    case NC_BACKEND_SSE2: return nc_sse2_tsc_overhead();
    case NC_BACKEND_SSE3: return nc_sse3_tsc_overhead();
    case NC_BACKEND_SSSE3: return nc_ssse3_tsc_overhead();
    case NC_BACKEND_SSE41: return nc_sse41_tsc_overhead();
    case NC_BACKEND_SSE42: return nc_sse42_tsc_overhead();
    case NC_BACKEND_AVX: return nc_avx_tsc_overhead();
    case NC_BACKEND_F16C: return nc_f16c_tsc_overhead();
    case NC_BACKEND_FMA: return nc_fma_tsc_overhead();
    case NC_BACKEND_AVX2: return nc_avx2_tsc_overhead();
    case NC_BACKEND_AVX_VNNI: return nc_avx_vnni_tsc_overhead();
    case NC_BACKEND_AVX512: return nc_avx512_tsc_overhead();
    case NC_BACKEND_AVX512VNNI: return nc_avx512_vnni_tsc_overhead();
    case NC_BACKEND_LEGACY:
    default: return nc_tsc_overhead();
    }
}
#  define _BE_START(b)  _be_start((b))
#  define _BE_END(b)    _be_end((b))
#endif

/* ─────────────────────────────────────────────────────────────────────────────
 * Context structure
 * ───────────────────────────────────────────────────────────────────────────── */
struct nc_ctx {
    nc_backend_t backend;
    uint64_t     tsc_hz;       /* calibrated TSC frequency (cycles / second) */
    uint64_t     qpc_hz;       /* QPC frequency */
    uint64_t     start_cycles; /* reference point for elapsed queries */
    uint64_t     start_qpc;
    /* rolling drift window */
    uint64_t     win_c0;
    uint64_t     win_q0;
    double       drift_ppm;
    /* overhead measurement cache */
    uint64_t     overhead_cyc;
};

/* ─────────────────────────────────────────────────────────────────────────────
 * AVX availability (CPU feature + OS XSAVE)
 * ───────────────────────────────────────────────────────────────────────────── */
static void _nc_cpuid(int out[4], int leaf, int subleaf) {
    __cpuidex(out, leaf, subleaf);
}

static int _nc_cpuid_max_leaf(void) {
    int info[4] = {0,0,0,0};
    _nc_cpuid(info, 0, 0);
    return info[0];
}

static int _nc_cpuid7_max_subleaf(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7) return 0;
    _nc_cpuid(i, 7, 0);
    return i[0];
}

static uint64_t _nc_xgetbv0_safe(void) {
    int info[4] = {0,0,0,0};
    _nc_cpuid(info, 1, 0);
    if (!((info[2] >> 27) & 1)) return 0; /* OSXSAVE */
    return (uint64_t)_xgetbv(0);
}

static int _nc_os_has_ymm_state(void) {
    int info[4] = {0,0,0,0};
    _nc_cpuid(info, 1, 0);
    if (!((info[2] >> 27) & 1)) return 0; /* OSXSAVE */
    if (!((info[2] >> 28) & 1)) return 0; /* AVX */
    return (_nc_xgetbv0_safe() & 0x6ULL) == 0x6ULL;
}

static int _nc_os_has_zmm_state(void) {
    return (_nc_xgetbv0_safe() & 0xE6ULL) == 0xE6ULL;
}

NC_API int nc_is_avx_available(void) { return _nc_os_has_ymm_state(); }

NC_API int nc_cpu_has_mmx(void)   { int i[4]; _nc_cpuid(i,1,0); return (i[3] >> 23) & 1; }
NC_API int nc_cpu_has_sse(void)   { int i[4]; _nc_cpuid(i,1,0); return (i[3] >> 25) & 1; }
NC_API int nc_cpu_has_sse2(void)  { int i[4]; _nc_cpuid(i,1,0); return (i[3] >> 26) & 1; }
NC_API int nc_cpu_has_sse3(void)  { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 0) & 1; }
NC_API int nc_cpu_has_pclmulqdq(void) { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 1) & 1; }
NC_API int nc_cpu_has_ssse3(void) { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 9) & 1; }
NC_API int nc_cpu_has_fma(void)   { int i[4]; _nc_cpuid(i,1,0); return ((i[2] >> 12) & 1) && _nc_os_has_ymm_state(); }
NC_API int nc_cpu_has_sse41(void) { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 19) & 1; }
NC_API int nc_cpu_has_sse42(void) { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 20) & 1; }
NC_API int nc_cpu_has_aesni(void) { int i[4]; _nc_cpuid(i,1,0); return (i[2] >> 25) & 1; }
NC_API int nc_cpu_has_f16c(void)  { int i[4]; _nc_cpuid(i,1,0); return ((i[2] >> 29) & 1) && _nc_os_has_ymm_state(); }

NC_API int nc_cpu_has_avx2(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_ymm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[1] >> 5) & 1;
}


NC_API int nc_cpu_has_avx_vnni(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || _nc_cpuid7_max_subleaf() < 1 || !_nc_os_has_ymm_state() || !nc_cpu_has_avx2()) return 0;
    _nc_cpuid(i, 7, 1);
    return (i[0] >> 4) & 1; /* CPUID.7.1.EAX[4] AVX-VNNI */
}

NC_API int nc_cpu_has_shani(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[1] >> 29) & 1;
}

NC_API int nc_cpu_has_vaes(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_ymm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[2] >> 9) & 1;
}

NC_API int nc_cpu_has_avx512f(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_zmm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[1] >> 16) & 1;
}
NC_API int nc_cpu_has_avx512bw(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_zmm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[1] >> 30) & 1;
}
NC_API int nc_cpu_has_avx512vl(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_zmm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[1] & (int)0x80000000u) != 0;
}
NC_API int nc_cpu_has_avx512vnni(void) {
    int i[4] = {0,0,0,0};
    if (_nc_cpuid_max_leaf() < 7 || !_nc_os_has_zmm_state()) return 0;
    _nc_cpuid(i, 7, 0);
    return (i[2] >> 11) & 1;
}

NC_API void nc_get_cpu_features(nc_cpu_features_t *out) {
    if (!out) return;
    memset(out, 0, sizeof(*out));
    out->mmx = nc_cpu_has_mmx();
    out->sse = nc_cpu_has_sse();
    out->sse2 = nc_cpu_has_sse2();
    out->sse3 = nc_cpu_has_sse3();
    out->ssse3 = nc_cpu_has_ssse3();
    out->sse41 = nc_cpu_has_sse41();
    out->sse42 = nc_cpu_has_sse42();
    out->aesni = nc_cpu_has_aesni();
    out->pclmulqdq = nc_cpu_has_pclmulqdq();
    out->shani = nc_cpu_has_shani();
    out->avx = nc_is_avx_available();
    out->f16c = nc_cpu_has_f16c();
    out->fma = nc_cpu_has_fma();
    out->avx2 = nc_cpu_has_avx2();
    out->avx_vnni = nc_cpu_has_avx_vnni();
    out->vaes = nc_cpu_has_vaes();
    out->avx512f = nc_cpu_has_avx512f();
    out->avx512bw = nc_cpu_has_avx512bw();
    out->avx512vl = nc_cpu_has_avx512vl();
    out->avx512vnni = nc_cpu_has_avx512vnni();
}

NC_API int nc_backend_is_available(nc_backend_t b) {
    switch (b) {
    case NC_BACKEND_LEGACY: return 1;
    case NC_BACKEND_MMX: return nc_cpu_has_mmx();
    case NC_BACKEND_SSE: return nc_cpu_has_sse();
    case NC_BACKEND_SSE2: return nc_cpu_has_sse2();
    case NC_BACKEND_SSE3: return nc_cpu_has_sse3();
    case NC_BACKEND_SSSE3: return nc_cpu_has_ssse3();
    case NC_BACKEND_SSE41: return nc_cpu_has_sse41();
    case NC_BACKEND_SSE42: return nc_cpu_has_sse42();
    case NC_BACKEND_AVX: return nc_is_avx_available();
    case NC_BACKEND_F16C: return nc_cpu_has_f16c();
    case NC_BACKEND_FMA: return nc_cpu_has_fma();
    case NC_BACKEND_AVX2: return nc_cpu_has_avx2();
    case NC_BACKEND_AVX_VNNI: return nc_cpu_has_avx_vnni();
    case NC_BACKEND_AVX512: return nc_cpu_has_avx512f();
    case NC_BACKEND_AVX512VNNI: return nc_cpu_has_avx512vnni();
    default: return 0;
    }
}

NC_API nc_backend_t nc_select_best_backend(void) {
    if (nc_cpu_has_avx512vnni()) return NC_BACKEND_AVX512VNNI;
    if (nc_cpu_has_avx512f()) return NC_BACKEND_AVX512;
    if (nc_cpu_has_avx_vnni()) return NC_BACKEND_AVX_VNNI;
    if (nc_cpu_has_avx2()) return NC_BACKEND_AVX2;
    if (nc_cpu_has_fma()) return NC_BACKEND_FMA;
    if (nc_cpu_has_f16c()) return NC_BACKEND_F16C;
    if (nc_is_avx_available()) return NC_BACKEND_AVX;
    if (nc_cpu_has_sse42()) return NC_BACKEND_SSE42;
    if (nc_cpu_has_sse41()) return NC_BACKEND_SSE41;
    if (nc_cpu_has_ssse3()) return NC_BACKEND_SSSE3;
    if (nc_cpu_has_sse3()) return NC_BACKEND_SSE3;
    if (nc_cpu_has_sse2()) return NC_BACKEND_SSE2;
    if (nc_cpu_has_sse()) return NC_BACKEND_SSE;
    if (nc_cpu_has_mmx()) return NC_BACKEND_MMX;
    return NC_BACKEND_LEGACY;
}

NC_API const char *nc_backend_name(nc_backend_t b) {
    switch (b) {
    case NC_BACKEND_LEGACY: return "legacy-asm";
    case NC_BACKEND_MMX: return "mmx";
    case NC_BACKEND_SSE: return "sse";
    case NC_BACKEND_SSE2: return "sse2";
    case NC_BACKEND_SSE3: return "sse3";
    case NC_BACKEND_SSSE3: return "ssse3";
    case NC_BACKEND_SSE41: return "sse4.1";
    case NC_BACKEND_SSE42: return "sse4.2";
    case NC_BACKEND_AVX: return "avx";
    case NC_BACKEND_F16C: return "f16c";
    case NC_BACKEND_FMA: return "fma";
    case NC_BACKEND_AVX2: return "avx2";
    case NC_BACKEND_AVX_VNNI: return "avx-vnni";
    case NC_BACKEND_AVX512: return "avx-512";
    case NC_BACKEND_AVX512VNNI: return "avx-512-vnni";
    default: return "unknown";
    }
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Calibration  (500 ms QPC window → TSC Hz)
 * ───────────────────────────────────────────────────────────────────────────── */
static uint64_t _calibrate(nc_ctx_t *ctx, uint32_t ms) {
    ctx->qpc_hz  = _qpc_freq();
    uint64_t q0  = _qpc_now();
    uint64_t t0  = _BE_START(ctx->backend);
    uint64_t tgt = (ctx->qpc_hz * ms) / 1000ULL;
    while ((_qpc_now() - q0) < tgt) _mm_pause();
    uint64_t t1  = _BE_END(ctx->backend);
    uint64_t q1  = _qpc_now();
    return _hz_from_window(t1 - t0, q1 - q0, ctx->qpc_hz);
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Lifecycle
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API nc_ctx_t *nc_create(void) {
#if defined(NC_AVX_BUILD)
    return nc_create_backend(NC_BACKEND_AVX);
#elif defined(NC_LEGACY_BUILD)
    return nc_create_backend(NC_BACKEND_LEGACY);
#else
    return nc_create_backend(nc_select_best_backend());
#endif
}

NC_API nc_ctx_t *nc_create_backend(nc_backend_t backend) {
#if defined(NC_AVX_BUILD)
    if (!nc_is_avx_available()) return NULL;
#endif
    nc_ctx_t *ctx = (nc_ctx_t *)calloc(1, sizeof(nc_ctx_t));
    if (!ctx) return NULL;
    if (!nc_backend_is_available(backend)) backend = nc_select_best_backend();
    if (!nc_backend_is_available(backend)) backend = NC_BACKEND_LEGACY;
    ctx->backend = backend;
    ctx->tsc_hz  = _calibrate(ctx, 500);
    if (!ctx->tsc_hz) ctx->tsc_hz = 1;
    /* measure ASM overhead */
    #if defined(NC_AVX_BUILD)
        ctx->overhead_cyc = nc_avx_tsc_overhead();
    #elif defined(NC_LEGACY_BUILD)
        ctx->overhead_cyc = nc_tsc_overhead();
    #else
        ctx->overhead_cyc = _be_overhead(backend);
#endif
    nc_reset(ctx);
    return ctx;
}

NC_API void nc_destroy(nc_ctx_t *ctx) { free(ctx); }

NC_API int nc_calibrate(nc_ctx_t *ctx, uint32_t ms) {
    if (!ctx) return 0;
    uint64_t hz = _calibrate(ctx, ms ? ms : 500);
    if (!hz) return 0;
    ctx->tsc_hz = hz;
    return 1;
}

NC_API void nc_reset(nc_ctx_t *ctx) {
    if (!ctx) return;
    ctx->start_cycles = _BE_END(ctx->backend);
    ctx->start_qpc    = _qpc_now();
    ctx->win_c0       = ctx->start_cycles;
    ctx->win_q0       = ctx->start_qpc;
    ctx->drift_ppm    = 0.0;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Core read / start / stop
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API uint64_t nc_start(nc_ctx_t *ctx) {
    if (!ctx) return 0;
    uint64_t c = _BE_START(ctx->backend);
    ctx->start_cycles = c;
    ctx->start_qpc    = _qpc_now();
    ctx->win_c0       = c;
    ctx->win_q0       = ctx->start_qpc;
    return c;
}
NC_API uint64_t nc_stop(nc_ctx_t *ctx)
    { return ctx ? _BE_END(ctx->backend) : 0; }
NC_API uint64_t nc_now_cycles(nc_ctx_t *ctx)
    { return ctx ? _BE_END(ctx->backend) : 0; }

/* ─────────────────────────────────────────────────────────────────────────────
 * Conversions
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API uint64_t nc_cycles_to_ns (nc_ctx_t *c, uint64_t cy) { return c ? _cycles_to_ns(cy, c->tsc_hz) : 0; }
NC_API uint64_t nc_cycles_to_us (nc_ctx_t *c, uint64_t cy) { return c ? _cycles_to_us(cy, c->tsc_hz) : 0; }
NC_API uint64_t nc_cycles_to_ms (nc_ctx_t *c, uint64_t cy) { return c ? _cycles_to_ms(cy, c->tsc_hz) : 0; }
NC_API double   nc_cycles_to_sec(nc_ctx_t *c, uint64_t cy) { return (c && c->tsc_hz) ? (double)cy/(double)c->tsc_hz : 0.0; }
NC_API uint64_t nc_ns_to_cycles (nc_ctx_t *c, uint64_t ns) { return c ? _ns_to_cycles(ns, c->tsc_hz) : 0; }

/* ─────────────────────────────────────────────────────────────────────────────
 * Elapsed helpers  (update drift window each call)
 * ───────────────────────────────────────────────────────────────────────────── */
static __forceinline uint64_t _elapsed_update(nc_ctx_t *ctx) {
    uint64_t now_c = _BE_END(ctx->backend);
    uint64_t now_q = _qpc_now();
    /* update drift window ~every second */
    uint64_t dq = now_q - ctx->win_q0;
    if (dq >= ctx->qpc_hz) {
        uint64_t dc    = now_c - ctx->win_c0;
        uint64_t inst  = _hz_from_window(dc, dq, ctx->qpc_hz);
        if (inst) {
            double diff = (double)(int64_t)(inst - ctx->tsc_hz);
            ctx->drift_ppm = (diff / (double)ctx->tsc_hz) * 1e6;
        }
        ctx->win_q0 = now_q;
        ctx->win_c0 = now_c;
    }
    return now_c - ctx->start_cycles;
}

NC_API uint64_t nc_elapsed_cycles(nc_ctx_t *c) { return c ? _elapsed_update(c) : 0; }
NC_API uint64_t nc_elapsed_ns    (nc_ctx_t *c) { return c ? _cycles_to_ns(_elapsed_update(c), c->tsc_hz) : 0; }
NC_API uint64_t nc_elapsed_us    (nc_ctx_t *c) { return c ? _cycles_to_us(_elapsed_update(c), c->tsc_hz) : 0; }
NC_API uint64_t nc_elapsed_ms    (nc_ctx_t *c) { return c ? _cycles_to_ms(_elapsed_update(c), c->tsc_hz) : 0; }
NC_API double   nc_elapsed_sec   (nc_ctx_t *c) { return c ? nc_cycles_to_sec(c, _elapsed_update(c)) : 0.0; }

/* ─────────────────────────────────────────────────────────────────────────────
 * Spin-sleep  (TSC-based precision)
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API void nc_sleep_ns(nc_ctx_t *ctx, uint64_t ns) {
    if (!ctx || !ns) return;
    uint64_t target = _BE_END(ctx->backend) + _ns_to_cycles(ns, ctx->tsc_hz);
    while (_BE_END(ctx->backend) < target) _mm_pause();
}
NC_API void nc_sleep_us(nc_ctx_t *ctx, uint64_t us) { nc_sleep_ns(ctx, us * 1000ULL); }

/* ─────────────────────────────────────────────────────────────────────────────
 * Info / diagnostics
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API uint64_t     nc_tsc_hz    (nc_ctx_t *c) { return c ? c->tsc_hz    : 0; }
NC_API double       nc_drift_ppm (nc_ctx_t *c) { return c ? c->drift_ppm : 0.0; }
NC_API nc_backend_t nc_backend   (nc_ctx_t *c) { return c ? c->backend   : NC_BACKEND_LEGACY; }

NC_API uint64_t nc_measure_overhead_cycles(nc_ctx_t *ctx) {
    if (!ctx) return 0;
    /* re-measure live (best-of run in ASM) */
    #if defined(NC_AVX_BUILD)
        uint64_t live = nc_avx_tsc_overhead();
    #elif defined(NC_LEGACY_BUILD)
        uint64_t live = nc_tsc_overhead();
    #else
        uint64_t live = _be_overhead(ctx->backend);
    #endif
    ctx->overhead_cyc = live;
    return live;
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Formatting  →  "hh:mm:ss:mmm:uuu:nnn"
 * ───────────────────────────────────────────────────────────────────────────── */
NC_API void nc_format_ns(uint64_t ns, char *buf, size_t cap) {
    if (!buf || !cap) return;
    uint64_t h  = ns / 3600000000000ULL;
    uint64_t m  = (ns /   60000000000ULL) % 60ULL;
    uint64_t s  = (ns /    1000000000ULL) % 60ULL;
    uint64_t ms = (ns /       1000000ULL) % 1000ULL;
    uint64_t us = (ns /          1000ULL) % 1000ULL;
    uint64_t n  =  ns                     % 1000ULL;
    snprintf(buf, cap, "%02llu:%02llu:%02llu:%03llu:%03llu:%03llu",
        (unsigned long long)h,  (unsigned long long)m,
        (unsigned long long)s,  (unsigned long long)ms,
        (unsigned long long)us, (unsigned long long)n);
}

NC_API void nc_format_elapsed(nc_ctx_t *ctx, char *buf, size_t cap) {
    nc_format_ns(nc_elapsed_ns(ctx), buf, cap);
}

/* ─────────────────────────────────────────────────────────────────────────────
 * DLL entry point  (only active in DLL builds)
 * ───────────────────────────────────────────────────────────────────────────── */
#if defined(NANOCHRONO_DLL) && defined(NANOCHRONO_EXPORTS)
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hInst);
    return TRUE;
}
#endif
