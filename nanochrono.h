/* =============================================================================
 * nanochrono.h  -  NanoChrono high-resolution TSC timing library
 *
 * Build modes:
 *   Static / unified exe :  include with no defines.
 *                           Link nc_legacy.obj + nc_avx.obj + nc_c_uni.obj.
 *                           nc_create() picks best backend at runtime.
 *   Dynamic (DLL user)   :  #define NANOCHRONO_DLL before including.
 *   DLL internals        :  also define NANOCHRONO_EXPORTS.
 * ============================================================================= */

#pragma once
#ifndef NANOCHRONO_H
#define NANOCHRONO_H

#include <stdint.h>
#include <stddef.h>

#ifdef NANOCHRONO_EXPORTS
#  define NC_API __declspec(dllexport)
#elif defined(NANOCHRONO_DLL)
#  define NC_API __declspec(dllimport)
#else
#  define NC_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ─────────────────────────────────────────────────────────────────────────────
 * Shared struct: packed RDTSCP read
 * ───────────────────────────────────────────────────────────────────────────── */
typedef struct nc_rdtscp_pair {
    uint64_t tsc;
    uint32_t aux;
    uint32_t _pad;
} nc_rdtscp_pair_t;

/* Sequential stamp: tsc + aux + monotone counter */
typedef struct nc_rdtscp_seq {
    uint64_t tsc;
    uint32_t aux;
    uint32_t seq;
} nc_rdtscp_seq_t;

/* ─────────────────────────────────────────────────────────────────────────────
 * LOW-LEVEL ASM  –  Legacy backend  (CPUID-serialised, SSE2 only)
 *   nanochrono.lib / nanochrono.dll
 * ───────────────────────────────────────────────────────────────────────────── */

/* GROUP 1 – Core TSC reads */
NC_API uint64_t nc_tsc_start(void);         /* CPUID + RDTSC               */
NC_API uint64_t nc_tsc_end(void);           /* RDTSCP + CPUID drain        */
NC_API uint64_t nc_tsc_raw(void);           /* bare RDTSC (no fence)       */
NC_API uint64_t nc_tsc_lfence_start(void);  /* LFENCE + RDTSC              */
NC_API uint64_t nc_tsc_lfence_end(void);    /* RDTSCP + LFENCE             */
NC_API uint64_t nc_tsc_mfence_start(void);  /* MFENCE + RDTSC              */
NC_API uint64_t nc_tsc_mfence_end(void);    /* RDTSCP + MFENCE             */
NC_API void     nc_tsc_serialize(void);     /* CPUID barrier only          */

/* GROUP 2 – RDTSCP pair */
NC_API uint32_t nc_tsc_aux(void);
NC_API void     nc_tsc_rdtscp_pair(nc_rdtscp_pair_t *out);

/* GROUP 3 – Delta / elapsed */
NC_API uint64_t nc_tsc_delta(uint64_t start, uint64_t end);
NC_API uint64_t nc_tsc_elapsed_from(uint64_t start);
NC_API uint64_t nc_tsc_rdtscp_elapsed(uint64_t start, uint32_t *aux_out);

/* GROUP 4 – Spin */
NC_API void     nc_tsc_spin_until(uint64_t target_cycles);

/* GROUP 5 – Overhead / invariant / CPUID */
NC_API uint64_t nc_tsc_overhead(void);
NC_API int      nc_tsc_invariant(void);
NC_API void     nc_tsc_cpu_brand(char buf[49]);
NC_API uint32_t nc_tsc_max_leaf(void);

/* GROUP 6 – SSE2 SIMD memory (MOVDQU 128-bit) */
NC_API void     nc_simd_zero_buf(void *dst, size_t bytes);
NC_API void     nc_simd_copy_buf(void *dst, const void *src, size_t bytes);
NC_API uint64_t nc_simd_xor_checksum(const void *ptr, size_t bytes);
NC_API uint64_t nc_simd_sum_u64(const uint64_t *ptr, size_t count);
NC_API void     nc_simd_nt_zero(void *dst_aligned, size_t bytes);   /* 16-byte aligned dst! */
NC_API void     nc_simd_prefetch_loop(const void *ptr, size_t bytes, size_t stride);

/* GROUP 7 – Prefetch-enhanced TSC */
NC_API uint64_t nc_tsc_prefetch_start(const void *prefetch_ptr); /* NULL = skip */
NC_API uint64_t nc_tsc_prefetch_end(const void *prefetch_ptr);   /* NULL = skip */
NC_API uint64_t nc_tsc_pause_start(void);   /* 16×PAUSE + CPUID + RDTSC    */
NC_API uint64_t nc_tsc_full_fence(void);    /* MFENCE + CPUID + LFENCE + RDTSC */

/* GROUP 8 – Sequential RDTSCP stamp */
NC_API void     nc_tsc_rdtscp_seq(nc_rdtscp_seq_t *stamp);

/* Backward compat */
NC_API uint64_t tsc_start_legacy(void);
NC_API uint64_t tsc_end_legacy(void);

/* ─────────────────────────────────────────────────────────────────────────────
 * LOW-LEVEL ASM  –  AVX backend  (LFENCE-serialised, AVX + SSE2)
 *   nanochrono_avx.lib / nanochrono_avx.dll
 * ───────────────────────────────────────────────────────────────────────────── */

/* GROUP 1 – Core TSC reads */
NC_API uint64_t nc_avx_tsc_start(void);         /* LFENCE + RDTSC              */
NC_API uint64_t nc_avx_tsc_end(void);           /* RDTSCP + LFENCE             */
NC_API uint64_t nc_avx_tsc_raw(void);           /* bare RDTSC                  */
NC_API uint64_t nc_avx_tsc_full_start(void);    /* VZEROALL + LFENCE + RDTSC   */
NC_API uint64_t nc_avx_tsc_full_end(void);      /* RDTSCP + LFENCE + VZEROALL  */
NC_API uint64_t nc_avx_tsc_upper_start(void);   /* VZEROUPPER + LFENCE + RDTSC */
NC_API uint64_t nc_avx_tsc_upper_end(void);     /* RDTSCP + LFENCE + VZEROUPPER*/
NC_API uint64_t nc_avx_tsc_sfence_start(void);  /* SFENCE + LFENCE + RDTSC     */
NC_API uint64_t nc_avx_tsc_sfence_end(void);    /* RDTSCP + LFENCE + SFENCE    */
NC_API uint64_t nc_avx_tsc_mfence_start(void);  /* MFENCE + LFENCE + RDTSC     */
NC_API uint64_t nc_avx_tsc_mfence_end(void);    /* RDTSCP + MFENCE + LFENCE    */

/* GROUP 2 – RDTSCP pair */
NC_API uint32_t nc_avx_tsc_aux(void);
NC_API void     nc_avx_tsc_rdtscp_pair(nc_rdtscp_pair_t *out);

/* GROUP 3 – Delta / elapsed */
NC_API uint64_t nc_avx_tsc_delta(uint64_t start, uint64_t end);
NC_API uint64_t nc_avx_tsc_elapsed_from(uint64_t start);

/* GROUP 4 – Spin */
NC_API void     nc_avx_spin_until(uint64_t target_cycles);

/* GROUP 5 – Overhead / invariant */
NC_API uint64_t nc_avx_tsc_overhead(void);
NC_API int      nc_avx_tsc_invariant(void);

/* GROUP 6 – YMM state */
NC_API void     nc_avx_vzeroall(void);
NC_API void     nc_avx_warmup(void);

/* GROUP 7 – SIMD memory (VMOVDQU 256-bit) */
NC_API void     nc_avx_simd_zero_buf(void *dst, size_t bytes);
NC_API void     nc_avx_simd_copy_buf(void *dst, const void *src, size_t bytes);
NC_API uint64_t nc_avx_simd_xor_checksum(const void *ptr, size_t bytes);
NC_API uint64_t nc_avx_simd_sum_u64(const uint64_t *ptr, size_t count);
NC_API void     nc_avx_simd_nt_zero(void *dst_aligned32, size_t bytes); /* 32-byte aligned! */
NC_API void     nc_avx_simd_prefetch_loop(const void *ptr, size_t bytes, size_t stride);

/* GROUP 8 – Prefetch-enhanced TSC */
NC_API uint64_t nc_avx_prefetch_tsc_start(const void *prefetch_ptr);
NC_API uint64_t nc_avx_prefetch_tsc_end(const void *prefetch_ptr);
NC_API uint64_t nc_avx_tsc_pause_start(void);  /* 16×PAUSE + LFENCE + RDTSC  */
NC_API uint64_t nc_avx_tsc_full_fence(void);   /* MFENCE+VZEROUPPER+LFENCE+RDTSC */

/* GROUP 9 – Sequential stamp */
NC_API void     nc_avx_tsc_rdtscp_seq(nc_rdtscp_seq_t *stamp);

/* GROUP 10 – CPU feature detection */
NC_API int      nc_avx_check(void);   /* 1 = AVX + OSXSAVE available */
NC_API int      nc_avx2_check(void);  /* 1 = AVX2 available          */

/* Backward compat */
NC_API uint64_t tsc_start_simd(void);
NC_API uint64_t tsc_end_simd(void);

/* ─────────────────────────────────────────────────────────────────────────────
 * HIGH-LEVEL C API  (exported from both DLLs)
 * ───────────────────────────────────────────────────────────────────────────── */

typedef enum {
    NC_BACKEND_LEGACY = 0,
    NC_BACKEND_AVX    = 1,
} nc_backend_t;

typedef struct nc_ctx nc_ctx_t;

/* Lifecycle */
NC_API nc_ctx_t *nc_create(void);
NC_API nc_ctx_t *nc_create_backend(nc_backend_t backend);
NC_API void      nc_destroy(nc_ctx_t *ctx);
NC_API int       nc_calibrate(nc_ctx_t *ctx, uint32_t ms);
NC_API void      nc_reset(nc_ctx_t *ctx);

/* Start / stop */
NC_API uint64_t  nc_start(nc_ctx_t *ctx);
NC_API uint64_t  nc_stop(nc_ctx_t *ctx);
NC_API uint64_t  nc_now_cycles(nc_ctx_t *ctx);

/* Conversion */
NC_API uint64_t  nc_cycles_to_ns(nc_ctx_t *ctx, uint64_t cycles);
NC_API uint64_t  nc_cycles_to_us(nc_ctx_t *ctx, uint64_t cycles);
NC_API uint64_t  nc_cycles_to_ms(nc_ctx_t *ctx, uint64_t cycles);
NC_API double    nc_cycles_to_sec(nc_ctx_t *ctx, uint64_t cycles);
NC_API uint64_t  nc_ns_to_cycles(nc_ctx_t *ctx, uint64_t ns);

/* Elapsed */
NC_API uint64_t  nc_elapsed_cycles(nc_ctx_t *ctx);
NC_API uint64_t  nc_elapsed_ns(nc_ctx_t *ctx);
NC_API uint64_t  nc_elapsed_us(nc_ctx_t *ctx);
NC_API uint64_t  nc_elapsed_ms(nc_ctx_t *ctx);
NC_API double    nc_elapsed_sec(nc_ctx_t *ctx);

/* Spin-sleep */
NC_API void      nc_sleep_ns(nc_ctx_t *ctx, uint64_t ns);
NC_API void      nc_sleep_us(nc_ctx_t *ctx, uint64_t us);

/* Info */
NC_API uint64_t      nc_tsc_hz(nc_ctx_t *ctx);
NC_API double        nc_drift_ppm(nc_ctx_t *ctx);
NC_API nc_backend_t  nc_backend(nc_ctx_t *ctx);
NC_API int           nc_is_avx_available(void);
NC_API uint64_t      nc_measure_overhead_cycles(nc_ctx_t *ctx);

/* Formatting  ->  "hh:mm:ss:mmm:uuu:nnn" */
NC_API void  nc_format_ns(uint64_t ns, char *buf, size_t cap);
NC_API void  nc_format_elapsed(nc_ctx_t *ctx, char *buf, size_t cap);

#ifdef __cplusplus
}
#endif
#endif /* NANOCHRONO_H */
