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

#if defined(_WIN32) && defined(NANOCHRONO_EXPORTS)
#  define NC_API __declspec(dllexport)
#elif defined(_WIN32) && defined(NANOCHRONO_DLL)
#  define NC_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(NANOCHRONO_EXPORTS)
#  define NC_API __attribute__((visibility("default")))
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
    NC_BACKEND_ASM_SCALAR = NC_BACKEND_LEGACY,
    NC_BACKEND_AVX    = 1,   /* preserved ABI value from previous releases */
    NC_BACKEND_MMX    = 2,
    NC_BACKEND_SSE,
    NC_BACKEND_SSE2,
    NC_BACKEND_SSE3,
    NC_BACKEND_SSSE3,
    NC_BACKEND_SSE41,
    NC_BACKEND_SSE42,
    NC_BACKEND_F16C,
    NC_BACKEND_FMA,
    NC_BACKEND_AVX2,
    NC_BACKEND_AVX_VNNI,
    NC_BACKEND_AVX512,
    NC_BACKEND_AVX512VNNI
} nc_backend_t;

typedef struct nc_ctx nc_ctx_t;

typedef struct nc_cpu_features {
    int mmx;
    int sse;
    int sse2;
    int sse3;
    int ssse3;
    int sse41;
    int sse42;
    int aesni;
    int pclmulqdq;
    int shani;
    int avx;
    int f16c;
    int fma;
    int avx2;
    int avx_vnni;
    int vaes;
    int avx512f;
    int avx512bw;
    int avx512vl;
    int avx512vnni;
} nc_cpu_features_t;

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
NC_API const char   *nc_backend_name(nc_backend_t backend);
NC_API int           nc_backend_is_available(nc_backend_t backend);
NC_API nc_backend_t  nc_select_best_backend(void);
NC_API int           nc_is_avx_available(void);
NC_API uint64_t      nc_measure_overhead_cycles(nc_ctx_t *ctx);
NC_API uint64_t      nc_measure_ffi_overhead_cycles(nc_ctx_t *ctx, uint32_t iterations);
NC_API uint64_t      nc_measure_call_overhead_cycles(nc_ctx_t *ctx, void (*fn)(void *), void *arg, uint32_t iterations);
NC_API int           nc_cpu_has_mmx(void);
NC_API int           nc_cpu_has_sse(void);
NC_API int           nc_cpu_has_sse2(void);
NC_API int           nc_cpu_has_sse3(void);
NC_API int           nc_cpu_has_ssse3(void);
NC_API int           nc_cpu_has_sse41(void);
NC_API int           nc_cpu_has_sse42(void);
NC_API int           nc_cpu_has_aesni(void);
NC_API int           nc_cpu_has_shani(void);
NC_API int           nc_cpu_has_vaes(void);
NC_API int           nc_cpu_has_pclmulqdq(void);
NC_API int           nc_cpu_has_f16c(void);
NC_API int           nc_cpu_has_fma(void);
NC_API int           nc_cpu_has_avx2(void);
NC_API int           nc_cpu_has_avx_vnni(void);
NC_API int           nc_cpu_has_avx512f(void);
NC_API int           nc_cpu_has_avx512bw(void);
NC_API int           nc_cpu_has_avx512vl(void);
NC_API int           nc_cpu_has_avx512vnni(void);
NC_API void          nc_get_cpu_features(nc_cpu_features_t *out);

/* Formatting  ->  "hh:mm:ss:mmm:uuu:nnn" */
NC_API void  nc_format_ns(uint64_t ns, char *buf, size_t cap);
NC_API void  nc_format_elapsed(nc_ctx_t *ctx, char *buf, size_t cap);

#ifdef __cplusplus
}
#endif
#endif /* NANOCHRONO_H */

/* -----------------------------------------------------------------------------
 * NanoChronometer Toolkit API
 * Swiss-army timing helpers for nanosecond/cycle profiling, FFI overhead,
 * crypto timing harnesses and defensive side-channel measurements.
 * -------------------------------------------------------------------------- */
#ifndef NANOCHRONO_TOOLKIT_API
#define NANOCHRONO_TOOLKIT_API 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nc_sample_stats {
    uint64_t count;
    uint64_t min_cycles;
    uint64_t max_cycles;
    uint64_t mean_cycles;
    uint64_t median_cycles;
    uint64_t p90_cycles;
    uint64_t p99_cycles;
    double variance_cycles;
    double stdev_cycles;
} nc_sample_stats_t;

typedef struct nc_timing_record {
    uint64_t start_cycles;
    uint64_t end_cycles;
    uint64_t delta_cycles;
    uint64_t delta_ns;
    uint32_t aux_start;
    uint32_t aux_end;
    uint32_t backend;
} nc_timing_record_t;

typedef void (*nc_void_callback_t)(void *arg);
typedef int  (*nc_crypto_callback_t)(void *out, size_t *out_len, const void *in, size_t in_len, void *arg);

NC_API const char *nc_toolkit_version(void);
NC_API uint64_t    nc_wall_time_ns(void);
NC_API uint64_t    nc_monotonic_time_ns(void);
NC_API uint64_t    nc_process_time_ns(void);
NC_API uint64_t    nc_thread_time_ns(void);

NC_API void     nc_compiler_barrier(void);
NC_API void     nc_memory_barrier(void);
NC_API void     nc_cpu_relax(void);
NC_API uint64_t nc_empty_call(void);
NC_API uint64_t nc_empty_call_u64(uint64_t x);

NC_API uint64_t nc_measure_callback_min_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters);
NC_API uint64_t nc_measure_callback_avg_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters);
NC_API uint64_t nc_measure_callback_ns(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters);
NC_API uint64_t nc_measure_dll_boundary_cycles(nc_ctx_t *ctx, uint32_t iters);
NC_API uint64_t nc_measure_python_wheel_boundary_cycles(nc_ctx_t *ctx, uint32_t iters);

NC_API int      nc_collect_samples_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint64_t *samples, uint32_t count);
NC_API int      nc_analyze_samples(const uint64_t *samples, uint32_t count, nc_sample_stats_t *out_stats);
NC_API double   nc_welch_t_score(const uint64_t *a, uint32_t na, const uint64_t *b, uint32_t nb);
NC_API uint64_t nc_median_cycles(uint64_t *samples, uint32_t count);
NC_API uint64_t nc_percentile_cycles(uint64_t *samples, uint32_t count, double percentile);

NC_API uint64_t nc_time_crypto_call_cycles(nc_ctx_t *ctx, nc_crypto_callback_t fn,
                                           void *out, size_t *out_len,
                                           const void *in, size_t in_len,
                                           void *arg, int *status_out);
NC_API int      nc_time_crypto_many(nc_ctx_t *ctx, nc_crypto_callback_t fn,
                                    void *out, size_t *out_len,
                                    const void *in, size_t in_len,
                                    void *arg, uint64_t *samples, uint32_t count);

NC_API uint64_t nc_time_memcmp_cycles(nc_ctx_t *ctx, const void *a, const void *b, size_t n, int *cmp_out);
NC_API uint64_t nc_time_constant_time_eq_cycles(nc_ctx_t *ctx, const void *a, const void *b, size_t n, int *eq_out);
NC_API int      nc_constant_time_eq(const void *a, const void *b, size_t n);
NC_API uint64_t nc_time_memory_load_cycles(nc_ctx_t *ctx, const void *ptr, size_t bytes, uint64_t *checksum_out);
NC_API uint64_t nc_time_memory_store_cycles(nc_ctx_t *ctx, void *ptr, size_t bytes, uint8_t value);
NC_API uint64_t nc_time_flush_reload_cycles(nc_ctx_t *ctx, const void *ptr);
NC_API uint64_t nc_time_prefetch_reload_cycles(nc_ctx_t *ctx, const void *ptr);

/* Optional low-level assembly symbols. x64 symbols live in .asm; ARM64 in .S. */
NC_API void     nc_asm_empty(void);
NC_API uint64_t nc_asm_ret0(void);
NC_API void     nc_asm_pause(void);
NC_API void     nc_asm_lfence(void);
NC_API void     nc_asm_sfence(void);
NC_API void     nc_asm_mfence(void);
NC_API void     nc_asm_prefetcht0(const void *ptr);
NC_API void     nc_asm_prefetcht1(const void *ptr);
NC_API void     nc_asm_prefetcht2(const void *ptr);
NC_API void     nc_asm_prefetchnta(const void *ptr);
NC_API void     nc_asm_clflush(const void *ptr);
NC_API void     nc_asm_clflushopt(const void *ptr);
NC_API uint64_t nc_asm_load_u64(const void *ptr);
NC_API void     nc_asm_store_u64(void *ptr, uint64_t value);
NC_API uint64_t nc_asm_xor_u64_stream(const void *ptr, size_t count);

NC_API uint64_t nc_arm64_cntvct(void);
NC_API uint64_t nc_arm64_cntfrq(void);
NC_API void     nc_arm64_isb(void);
NC_API void     nc_arm64_dmb_sy(void);
NC_API void     nc_arm64_dsb_sy(void);
NC_API void     nc_arm64_yield(void);
NC_API uint64_t nc_arm64_load_u64(const void *ptr);
NC_API void     nc_arm64_store_u64(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_xor_u64_stream(const void *ptr, size_t count);


/* Unified instruction-family timing API: one nanochrono library per OS/architecture.
 * Runtime dispatch prevents illegal instructions: unsupported instruction families
 * return NC_ERR_UNSUPPORTED instead of being executed. */
typedef enum nc_instruction_family {
    NC_INSTR_SCALAR = 0, NC_INSTR_AES, NC_INSTR_SHA, NC_INSTR_PCLMULQDQ,
    NC_INSTR_CRC32, NC_INSTR_SSE2, NC_INSTR_AVX2, NC_INSTR_AVX512,
    NC_INSTR_NEON, NC_INSTR_SVE, NC_INSTR_SVE2, NC_INSTR_SME, NC_INSTR_SME2
} nc_instruction_family_t;
typedef enum nc_status_code { NC_OK = 0, NC_ERR_UNSUPPORTED = -1, NC_ERR_BAD_ARGUMENT = -2, NC_ERR_CRYPTO_BACKEND = -3 } nc_status_code_t;
typedef struct nc_instruction_result { int status; uint32_t family; uint32_t backend; uint64_t cycles; uint64_t ns; uint64_t blocks; uint64_t checksum; } nc_instruction_result_t;
typedef struct nc_sidechannel_result { int status; uint64_t cached_cycles; uint64_t flushed_cycles; uint64_t prefetched_cycles; uint64_t threshold_cycles; double separation_score; } nc_sidechannel_result_t;
NC_API const char *nc_instruction_family_name(nc_instruction_family_t family);
NC_API int      nc_instruction_family_available(nc_instruction_family_t family);
NC_API uint64_t nc_measure_instruction_family_cycles(nc_ctx_t *ctx, nc_instruction_family_t family, uint32_t iterations, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_aesenc_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_sha256msg_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_pclmul_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_vector_add_cycles(nc_ctx_t *ctx, nc_instruction_family_t family, const void *a, const void *b, void *out_buf, size_t bytes, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_vector_xor_cycles(nc_ctx_t *ctx, nc_instruction_family_t family, const void *a, const void *b, void *out_buf, size_t bytes, nc_instruction_result_t *out);
NC_API uint64_t nc_measure_cache_probe_cycles(nc_ctx_t *ctx, const void *ptr, nc_sidechannel_result_t *out);
NC_API uint64_t nc_measure_branch_mispredict_cycles(nc_ctx_t *ctx, const uint8_t *pattern, size_t count, nc_instruction_result_t *out);
NC_API int      nc_crypto_backend_mask(void);
NC_API uint64_t nc_crypto_rand_cycles(nc_ctx_t *ctx, void *out_buf, size_t bytes, nc_instruction_result_t *out);
NC_API uint64_t nc_crypto_sha256_cycles(nc_ctx_t *ctx, const void *msg, size_t len, uint8_t out_digest[32], nc_instruction_result_t *out);
NC_API uint64_t nc_crypto_hmac_sha256_cycles(nc_ctx_t *ctx, const void *key, size_t key_len, const void *msg, size_t msg_len, uint8_t out_mac[32], nc_instruction_result_t *out);
NC_API uint64_t nc_crypto_aead_chacha20poly1305_cycles(nc_ctx_t *ctx, const uint8_t *key, const uint8_t *nonce, const uint8_t *aad, size_t aad_len, const uint8_t *msg, size_t msg_len, uint8_t *cipher, uint8_t tag[16], nc_instruction_result_t *out);
NC_API uint64_t nc_asm_aesenc_128_loop(uint32_t blocks);
NC_API uint64_t nc_asm_aesdec_128_loop(uint32_t blocks);
NC_API uint64_t nc_asm_pclmul_loop(uint32_t blocks);
NC_API uint64_t nc_asm_sha256rnds2_loop(uint32_t blocks);
NC_API uint64_t nc_asm_crc32_loop(const void *ptr, size_t bytes);
NC_API uint64_t nc_asm_vector_add_u64_loop(const void *a, const void *b, void *out_buf, size_t count);
NC_API uint64_t nc_asm_vector_xor_loop(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_asm_branch_probe_loop(const uint8_t *pattern, size_t count);
NC_API uint64_t nc_arm64_neon_aes_loop(uint32_t blocks);
NC_API uint64_t nc_arm64_neon_sha256_loop(uint32_t blocks);
NC_API uint64_t nc_arm64_neon_pmull_loop(uint32_t blocks);
NC_API uint64_t nc_arm64_neon_vector_xor_loop(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_sve_vector_xor_loop(const void *a, const void *b, void *out_buf, size_t bytes);

/* -----------------------------------------------------------------------------
 * Professional benchmark/side-channel audit API
 *
 * These entry points are intended for defensive constant-time auditing and local
 * microbenchmarking. They do not implement secret recovery, cross-process victim
 * probing, or exploit orchestration. The catalog exposes many low-level white-box
 * kernels while nc_microbench_available() gates optional ISA families before use.
 * -------------------------------------------------------------------------- */
typedef enum nc_benchmark_mode {
    NC_BENCHMARK_BLACK_BOX_CRYPTO = 0,  /* EVP/libsodium/BoringSSL public APIs */
    NC_BENCHMARK_GRAY_BOX_INTERNAL = 1, /* internal block/kernels when available */
    NC_BENCHMARK_WHITE_BOX_ASM = 2      /* direct assembly microkernels */
} nc_benchmark_mode_t;

typedef uint64_t (*nc_microbench_fn_t)(uint32_t iterations, void *buffer, size_t bytes);

enum {
    NC_MICROBENCH_F_NONE     = 0u,
    NC_MICROBENCH_F_SSE2     = 1u << 0,
    NC_MICROBENCH_F_AESNI    = 1u << 1,
    NC_MICROBENCH_F_PCLMUL   = 1u << 2,
    NC_MICROBENCH_F_SHANI    = 1u << 3,
    NC_MICROBENCH_F_AVX2     = 1u << 4,
    NC_MICROBENCH_F_AVX512F  = 1u << 5,
    NC_MICROBENCH_F_NEON     = 1u << 6,
    NC_MICROBENCH_F_ARMCRYPTO= 1u << 7
};

typedef struct nc_microbench_descriptor {
    const char *name;
    const char *arch;
    const char *family;
    const char *category;
    nc_benchmark_mode_t mode;
    uint32_t feature_flags;
    nc_microbench_fn_t fn;
} nc_microbench_descriptor_t;

typedef struct nc_microbench_result {
    int status;
    uint64_t cycles;
    uint64_t ns;
    uint64_t checksum;
    uint32_t iterations;
    uint32_t catalog_index;
} nc_microbench_result_t;

typedef struct nc_stopwatch_ns {
    uint64_t start_ns;
    uint64_t last_lap_ns;
    uint64_t accumulated_ns;
    int running;
} nc_stopwatch_ns_t;

typedef void (*nc_sidechannel_candidate_t)(uint8_t *out, size_t out_len,
                                           const uint8_t *in, size_t in_len,
                                           void *arg);

typedef struct nc_sidechannel_audit_config {
    uint32_t samples;        /* default 2000 */
    size_t input_len;        /* default 64 */
    size_t output_len;       /* default 64 */
    uint32_t warmup;         /* default 64 */
    uint32_t fixed_seed;     /* deterministic local test vectors */
    uint8_t fixed_value;     /* fixed-input byte pattern */
} nc_sidechannel_audit_config_t;

typedef struct nc_sidechannel_audit_result {
    int status;
    uint32_t samples;
    nc_sample_stats_t fixed_stats;
    nc_sample_stats_t random_stats;
    double welch_t;
    double abs_welch_t;
    int likely_timing_leak;      /* heuristic: |t| >= 4.5 */
    uint64_t mean_delta_cycles;
    uint64_t mean_delta_ns;
} nc_sidechannel_audit_result_t;

NC_API const char *nc_benchmark_mode_name(nc_benchmark_mode_t mode);
NC_API size_t nc_microbench_catalog_count(void);
NC_API const nc_microbench_descriptor_t *nc_microbench_catalog_entry(size_t index);
NC_API int nc_microbench_available(const nc_microbench_descriptor_t *desc);
NC_API uint64_t nc_microbench_run(nc_ctx_t *ctx, size_t index, uint32_t iterations,
                                  void *buffer, size_t bytes, nc_microbench_result_t *out);
NC_API uint64_t nc_benchmark_mode_cycles(nc_ctx_t *ctx, nc_benchmark_mode_t mode,
                                         nc_instruction_family_t family,
                                         uint32_t iterations, const void *input,
                                         size_t input_len, nc_instruction_result_t *out);

NC_API void nc_stopwatch_ns_reset(nc_stopwatch_ns_t *sw);
NC_API void nc_stopwatch_ns_start(nc_stopwatch_ns_t *sw);
NC_API uint64_t nc_stopwatch_ns_lap(nc_stopwatch_ns_t *sw);
NC_API uint64_t nc_stopwatch_ns_stop(nc_stopwatch_ns_t *sw);
NC_API uint64_t nc_stopwatch_ns_elapsed(const nc_stopwatch_ns_t *sw);

NC_API int nc_sidechannel_audit_constant_time(nc_ctx_t *ctx,
                                              nc_sidechannel_candidate_t candidate,
                                              void *arg,
                                              const nc_sidechannel_audit_config_t *config,
                                              nc_sidechannel_audit_result_t *out);
NC_API uint64_t nc_measure_load_probe_asm_cycles(nc_ctx_t *ctx, const void *ptr, nc_sidechannel_result_t *out);

/* Low-level assembly side-channel measurement API.
 * These functions expose local timing/cache/branch primitives for defensive
 * benchmarking. They measure only caller-provided buffers and do not implement
 * secret recovery or cross-process probing. Crypto remains delegated to EVP,
 * BoringSSL or libsodium through the black-box crypto API above. */
typedef enum nc_asm_sidechannel_probe_kind {
    NC_ASM_SC_PROBE_LOAD = 0,
    NC_ASM_SC_PROBE_LOAD_FENCED = 1,
    NC_ASM_SC_PROBE_FLUSH_RELOAD = 2,      /* x64 only; unsupported on ARM64 */
    NC_ASM_SC_PROBE_PREFETCH_RELOAD = 3,
    NC_ASM_SC_PROBE_STORE = 4,
    NC_ASM_SC_PROBE_BRANCH = 5,
    NC_ASM_SC_PROBE_POINTER_CHASE = 6,
    NC_ASM_SC_PROBE_BARRIER = 7,
    NC_ASM_SC_PROBE_COUNTER = 8
} nc_asm_sidechannel_probe_kind_t;

typedef enum nc_asm_sidechannel_arch {
    NC_ASM_SC_ARCH_NONE = 0,
    NC_ASM_SC_ARCH_X64 = 1,
    NC_ASM_SC_ARCH_ARM64 = 2
} nc_asm_sidechannel_arch_t;

typedef struct nc_asm_sidechannel_probe_result {
    int status;
    uint32_t arch;
    uint32_t kind;
    uint64_t raw_units;      /* x64: TSC cycles; ARM64: CNTVCT ticks */
    uint64_t ns;             /* converted when ctx/frequency is available */
    uint64_t aux;            /* TSC_AUX on x64 RDTSCP; reserved on ARM64 */
    uint64_t checksum;       /* reserved for future buffer-integrity checks */
} nc_asm_sidechannel_probe_result_t;

NC_API const char *nc_asm_sidechannel_probe_name(nc_asm_sidechannel_probe_kind_t kind);
NC_API int nc_asm_sidechannel_arch(void);
NC_API uint64_t nc_asm_sidechannel_probe(nc_ctx_t *ctx,
                                         nc_asm_sidechannel_probe_kind_t kind,
                                         void *buffer,
                                         size_t bytes_or_steps,
                                         uint64_t value,
                                         nc_asm_sidechannel_probe_result_t *out);
NC_API int nc_asm_sidechannel_cache_audit(nc_ctx_t *ctx,
                                          void *buffer,
                                          nc_asm_sidechannel_probe_result_t *load_out,
                                          nc_asm_sidechannel_probe_result_t *reload_out,
                                          nc_asm_sidechannel_probe_result_t *prefetch_out);

/* Direct x64 assembly entry points. */
NC_API uint64_t nc_x64_sc_rdtsc_lfence(void);
NC_API uint64_t nc_x64_sc_rdtscp_lfence(uint32_t *aux_out);
NC_API uint64_t nc_x64_sc_measure_load_cycles(const void *ptr);
NC_API uint64_t nc_x64_sc_measure_load_lfence_cycles(const void *ptr);
NC_API uint64_t nc_x64_sc_measure_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_x64_sc_measure_flush_reload_cycles(const void *ptr);
NC_API uint64_t nc_x64_sc_measure_prefetch_reload_cycles(const void *ptr);
NC_API uint64_t nc_x64_sc_measure_branch_cycles(const uint8_t *pattern, size_t count);
NC_API uint64_t nc_x64_sc_pointer_chase_cycles(const void *first, size_t steps);
NC_API uint64_t nc_x64_sc_barrier_overhead_cycles(uint32_t iterations);

/* Direct ARM64 assembly entry points. These return CNTVCT ticks. */
NC_API uint64_t nc_arm64_sc_cntvct_isb(void);
NC_API uint64_t nc_arm64_sc_measure_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sc_measure_load_isb_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sc_measure_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_sc_measure_prefetch_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sc_measure_branch_ticks(const uint8_t *pattern, size_t count);
NC_API uint64_t nc_arm64_sc_pointer_chase_ticks(const void *first, size_t steps);
NC_API uint64_t nc_arm64_sc_barrier_overhead_ticks(uint32_t iterations);


/* Low-level SIMD assembly timing API.
 * These symbols are implemented in every SIMD assembler family file, not only
 * in scalar or legacy sources. They expose local-buffer measurement primitives
 * for defensive benchmarking and side-channel hardening audits. */
typedef enum nc_asm_simd_family {
    NC_ASM_SIMD_MMX = 0,
    NC_ASM_SIMD_SSE,
    NC_ASM_SIMD_SSE2,
    NC_ASM_SIMD_SSE3,
    NC_ASM_SIMD_SSSE3,
    NC_ASM_SIMD_SSE41,
    NC_ASM_SIMD_SSE42,
    NC_ASM_SIMD_F16C,
    NC_ASM_SIMD_FMA,
    NC_ASM_SIMD_AVX,
    NC_ASM_SIMD_AVX2,
    NC_ASM_SIMD_AVX_VNNI,
    NC_ASM_SIMD_AVX512,
    NC_ASM_SIMD_AVX512_VNNI,
    NC_ASM_SIMD_ARM64_NEON,
    NC_ASM_SIMD_ARM64_SVE,
    NC_ASM_SIMD_ARM64_SVE2,
    NC_ASM_SIMD_ARM64_SME,
    NC_ASM_SIMD_ARM64_SME2
} nc_asm_simd_family_t;

typedef enum nc_asm_simd_probe_kind {
    NC_ASM_SIMD_PROBE_COUNTER = 0,
    NC_ASM_SIMD_PROBE_LOAD = 1,
    NC_ASM_SIMD_PROBE_STORE = 2,
    NC_ASM_SIMD_PROBE_VECTOR_LOAD = 3,
    NC_ASM_SIMD_PROBE_VECTOR_XOR = 4,
    NC_ASM_SIMD_PROBE_BARRIER = 5
} nc_asm_simd_probe_kind_t;

typedef struct nc_asm_simd_probe_result {
    int status;
    uint32_t family;
    uint32_t kind;
    uint64_t raw_units;
    uint64_t ns;
    uint64_t bytes;
    uint64_t checksum;
} nc_asm_simd_probe_result_t;

NC_API const char *nc_asm_simd_family_name(nc_asm_simd_family_t family);
NC_API const char *nc_asm_simd_probe_name(nc_asm_simd_probe_kind_t kind);
NC_API int nc_asm_simd_family_available(nc_asm_simd_family_t family);
NC_API uint64_t nc_asm_simd_probe(nc_ctx_t *ctx,
                                  nc_asm_simd_family_t family,
                                  nc_asm_simd_probe_kind_t kind,
                                  void *a,
                                  void *b,
                                  void *out_buf,
                                  size_t bytes_or_iterations,
                                  uint64_t value,
                                  nc_asm_simd_probe_result_t *out);

/* Direct x64 SIMD assembly entry points. */
NC_API uint64_t nc_mmx_sc_counter(void);
NC_API uint64_t nc_mmx_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_mmx_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_mmx_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_mmx_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_mmx_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_sse_sc_counter(void);
NC_API uint64_t nc_sse_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_sse_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_sse_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_sse_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_sse_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_sse2_sc_counter(void);
NC_API uint64_t nc_sse2_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_sse2_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_sse2_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_sse2_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_sse2_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_sse3_sc_counter(void);
NC_API uint64_t nc_sse3_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_sse3_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_sse3_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_sse3_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_sse3_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_ssse3_sc_counter(void);
NC_API uint64_t nc_ssse3_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_ssse3_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_ssse3_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_ssse3_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_ssse3_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_sse41_sc_counter(void);
NC_API uint64_t nc_sse41_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_sse41_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_sse41_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_sse41_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_sse41_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_sse42_sc_counter(void);
NC_API uint64_t nc_sse42_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_sse42_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_sse42_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_sse42_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_sse42_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_f16c_sc_counter(void);
NC_API uint64_t nc_f16c_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_f16c_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_f16c_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_f16c_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_f16c_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_fma_sc_counter(void);
NC_API uint64_t nc_fma_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_fma_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_fma_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_fma_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_fma_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_avx_sc_counter(void);
NC_API uint64_t nc_avx_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_avx_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_avx_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_avx_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_avx_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_avx2_sc_counter(void);
NC_API uint64_t nc_avx2_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_avx2_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_avx2_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_avx2_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_avx2_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_avx_vnni_sc_counter(void);
NC_API uint64_t nc_avx_vnni_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_avx_vnni_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_avx_vnni_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_avx_vnni_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_avx_vnni_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_avx512_sc_counter(void);
NC_API uint64_t nc_avx512_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_avx512_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_avx512_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_avx512_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_avx512_sc_barrier_cycles(uint32_t iterations);
NC_API uint64_t nc_avx512_vnni_sc_counter(void);
NC_API uint64_t nc_avx512_vnni_sc_load_cycles(const void *ptr);
NC_API uint64_t nc_avx512_vnni_sc_store_cycles(void *ptr, uint64_t value);
NC_API uint64_t nc_avx512_vnni_sc_vector_load_cycles(const void *ptr);
NC_API uint64_t nc_avx512_vnni_sc_vector_xor_cycles(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_avx512_vnni_sc_barrier_cycles(uint32_t iterations);

/* Direct ARM64 SIMD assembly entry points. These return CNTVCT ticks. */
NC_API uint64_t nc_arm64_neon_sc_counter(void);
NC_API uint64_t nc_arm64_neon_sc_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_neon_sc_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_neon_sc_vector_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_neon_sc_vector_xor_ticks(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_neon_sc_barrier_ticks(uint32_t iterations);
NC_API uint64_t nc_arm64_sve_sc_counter(void);
NC_API uint64_t nc_arm64_sve_sc_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sve_sc_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_sve_sc_vector_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sve_sc_vector_xor_ticks(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_sve_sc_barrier_ticks(uint32_t iterations);
NC_API uint64_t nc_arm64_sve2_sc_counter(void);
NC_API uint64_t nc_arm64_sve2_sc_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sve2_sc_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_sve2_sc_vector_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sve2_sc_vector_xor_ticks(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_sve2_sc_barrier_ticks(uint32_t iterations);
NC_API uint64_t nc_arm64_sme_sc_counter(void);
NC_API uint64_t nc_arm64_sme_sc_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sme_sc_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_sme_sc_vector_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sme_sc_vector_xor_ticks(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_sme_sc_barrier_ticks(uint32_t iterations);
NC_API uint64_t nc_arm64_sme2_sc_counter(void);
NC_API uint64_t nc_arm64_sme2_sc_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sme2_sc_store_ticks(void *ptr, uint64_t value);
NC_API uint64_t nc_arm64_sme2_sc_vector_load_ticks(const void *ptr);
NC_API uint64_t nc_arm64_sme2_sc_vector_xor_ticks(const void *a, const void *b, void *out_buf, size_t bytes);
NC_API uint64_t nc_arm64_sme2_sc_barrier_ticks(uint32_t iterations);



/* -----------------------------------------------------------------------------
 * Nanosecond clock, low-level counter routes, auto SIMD route selection, and
 * wrapper/FFI overhead API.
 * -------------------------------------------------------------------------- */
#ifndef NANOCHRONO_NANOCLOCK_API
#define NANOCHRONO_NANOCLOCK_API 1

typedef enum nc_clock_route {
    NC_CLOCK_ROUTE_AUTO = 0,
    NC_CLOCK_ROUTE_WALL_REALTIME_NS,
    NC_CLOCK_ROUTE_MONOTONIC_NS,
    NC_CLOCK_ROUTE_X64_RDTSC_RAW,
    NC_CLOCK_ROUTE_X64_RDTSC_LFENCE,
    NC_CLOCK_ROUTE_X64_RDTSC_MFENCE,
    NC_CLOCK_ROUTE_X64_RDTSCP_LFENCE,
    NC_CLOCK_ROUTE_ARM64_CNTFRQ_EL0,
    NC_CLOCK_ROUTE_ARM64_CNTVCT_EL0,
    NC_CLOCK_ROUTE_ARM64_CNTVCT_ISB,
    NC_CLOCK_ROUTE_ARM64_PMCCNTR_EL0,
    NC_CLOCK_ROUTE_BEST_SIMD_COUNTER
} nc_clock_route_t;

typedef enum nc_wrapper_kind {
    NC_WRAPPER_C_API = 0,
    NC_WRAPPER_DLL_BOUNDARY,
    NC_WRAPPER_PYTHON_CTYPES,
    NC_WRAPPER_PYTHON_CFFI,
    NC_WRAPPER_NODE_FFI,
    NC_WRAPPER_LUA_FFI,
    NC_WRAPPER_JAVA_JNA,
    NC_WRAPPER_CSHARP_PINVOKE,
    NC_WRAPPER_GO_CGO,
    NC_WRAPPER_RUST_FFI,
    NC_WRAPPER_ZIG_FFI
} nc_wrapper_kind_t;

typedef struct nc_nanoclock_snapshot {
    int status;
    uint32_t arch;
    uint32_t route;
    uint32_t best_simd_family;
    uint32_t backend;
    uint64_t unix_time_ns;
    uint64_t monotonic_ns;
    uint64_t process_time_ns;
    uint64_t thread_time_ns;
    uint64_t selected_raw_units;
    uint64_t selected_ns;
    uint64_t selected_overhead_units;
    uint64_t rdtsc_raw;
    uint64_t rdtsc_lfence;
    uint64_t rdtsc_mfence;
    uint64_t rdtscp_lfence;
    uint32_t rdtscp_aux;
    uint32_t _pad0;
    uint64_t arm64_cntfrq_el0;
    uint64_t arm64_cntvct_el0;
    uint64_t arm64_cntvct_isb;
    uint64_t arm64_cntvct_ns;
    uint64_t arm64_pmccntr_el0;
    uint64_t arm64_pmccntr_ns;
    uint32_t arm64_pmccntr_available;
    uint32_t arm64_pmccntr_status;
    uint64_t best_simd_counter;
    uint64_t best_simd_counter_ns;
    uint64_t native_overhead_cycles;
    uint64_t ffi_overhead_cycles;
    uint64_t dll_boundary_cycles;
    uint64_t wrapper_hint_cycles;
} nc_nanoclock_snapshot_t;

NC_API uint64_t nc_unix_time_ns(void);
NC_API const char *nc_clock_route_name(nc_clock_route_t route);
NC_API const char *nc_wrapper_kind_name(nc_wrapper_kind_t kind);
NC_API nc_asm_simd_family_t nc_select_best_simd_family(void);
NC_API nc_clock_route_t nc_clock_select_best_route(void);
NC_API uint64_t nc_nanoclock_now_ns(nc_ctx_t *ctx, nc_clock_route_t route, uint64_t *raw_units_out);
NC_API int nc_nanoclock_snapshot(nc_ctx_t *ctx, nc_nanoclock_snapshot_t *out);
NC_API void nc_format_unix_time_ns(uint64_t unix_ns, char *buf, size_t cap);
NC_API uint64_t nc_measure_wrapper_overhead_cycles(nc_ctx_t *ctx, nc_wrapper_kind_t kind, uint32_t iterations);
NC_API uint64_t nc_x64_sc_rdtsc_mfence(void);
NC_API uint64_t nc_arm64_pmccntr_el0(void);
NC_API int nc_arm64_pmccntr_el0_available(void);

#endif /* NANOCHRONO_NANOCLOCK_API */

/* -----------------------------------------------------------------------------
 * Android ARM64 execution-context API.
 *
 * The ARM64 .S/.asm files remain pure timing/probe kernels: CNTVCT nanosecond
 * clock reads, side-channel/local latency probes, interpreted-language wrapper
 * latency, and other non-crypto microbenchmarks. Android privilege, root
 * consent, perf_event/PMU availability, and future helper-process selection are
 * runtime policy/backend concerns handled by this API. Crypto stays in
 * externals/arm64/android through BoringSSL/libsodium C backends.
 * -------------------------------------------------------------------------- */
#ifndef NANOCHRONO_ANDROID_ARM64_API
#define NANOCHRONO_ANDROID_ARM64_API 1

typedef enum nc_android_privilege {
    NC_ANDROID_PRIVILEGE_USER_APP = 0,
    NC_ANDROID_PRIVILEGE_ROOT_PROMPTABLE = 1,
    NC_ANDROID_PRIVILEGE_ROOT_GRANTED = 2,
    NC_ANDROID_PRIVILEGE_KERNEL_ASSISTED = 3,
    NC_ANDROID_PRIVILEGE_DENIED = 4
} nc_android_privilege_t;

typedef enum nc_android_backend {
    NC_ANDROID_BACKEND_NONE = 0,
    NC_ANDROID_BACKEND_USER_CNTVCT = 1,
    NC_ANDROID_BACKEND_USER_PERF_EVENT = 2,
    NC_ANDROID_BACKEND_ROOT_PERF_EVENT = 3,
    NC_ANDROID_BACKEND_SIMPLEPERF_IMPORT = 4,
    NC_ANDROID_BACKEND_KERNEL_ASSISTED = 5
} nc_android_backend_t;

typedef enum nc_android_mode {
    NC_ANDROID_MODE_AUTO = 0,
    NC_ANDROID_MODE_USER_ONLY = 1,
    NC_ANDROID_MODE_ROOT_IF_CONSENTS = 2,
    NC_ANDROID_MODE_REQUIRE_ROOT = 3
} nc_android_mode_t;

typedef struct nc_android_caps {
    uint32_t struct_size;
    uint32_t privilege;
    uint32_t recommended_backend;
    uint32_t current_process_root;

    uint32_t has_cntvct_el0;
    uint32_t has_neon;
    uint32_t has_sve;
    uint32_t has_sve2;
    uint32_t has_sme;
    uint32_t has_sme2;

    uint32_t perf_event_available;
    uint32_t perf_event_restricted;
    uint32_t perf_hw_cycles_available;
    uint32_t perf_instructions_available;
    uint32_t perf_cache_misses_available;
    uint32_t perf_branch_misses_available;

    uint32_t root_prompt_available;
    uint32_t root_granted;
    uint32_t simpleperf_available;
    uint32_t reserved0;

    char abi[32];
    char kernel_release[64];
    char device_model[64];
} nc_android_caps_t;

typedef struct nc_android_config {
    uint32_t mode;
    uint32_t allow_su_prompt;
    uint32_t allow_pmu_counters;
    uint32_t allow_kernel_samples;
    uint32_t allow_system_wide_profile;
    uint32_t prefer_simpleperf;
    uint32_t sample_frequency_hz;
    uint32_t reserved0;
} nc_android_config_t;

typedef struct nc_android_root_consent_result {
    uint32_t struct_size;
    uint32_t prompt_attempted;
    uint32_t granted;
    int32_t exit_code;
    char message[160];
} nc_android_root_consent_result_t;

typedef struct nc_android_backend_info {
    uint32_t struct_size;
    uint32_t privilege;
    uint32_t backend;
    uint32_t used_su;
    uint32_t user_granted_root;
    uint32_t pmu_available;
    uint32_t pmu_restricted;
    uint32_t requires_helper_process;
    uint32_t fallback_used;
    uint32_t reserved0;
    char reason[192];
} nc_android_backend_info_t;

NC_API const char *nc_android_privilege_name(nc_android_privilege_t privilege);
NC_API const char *nc_android_backend_name(nc_android_backend_t backend);
NC_API const char *nc_android_mode_name(nc_android_mode_t mode);
NC_API int nc_android_default_config(nc_android_config_t *cfg);
NC_API int nc_android_detect_capabilities(nc_android_caps_t *out);

/* Explicit user-consent gate. This is the first privileged operation for root
 * mode and intentionally invokes `su -c id` so Magisk/SuperSU can ask the user.
 * NanoChronometer does not silently elevate, bypass, or persist root access. */
NC_API int nc_android_request_root_consent(nc_android_root_consent_result_t *out);

NC_API int nc_android_select_backend(const nc_android_config_t *cfg,
                                     const nc_android_caps_t *caps,
                                     nc_android_backend_info_t *out);

#endif /* NANOCHRONO_ANDROID_ARM64_API */

/* -----------------------------------------------------------------------------
 * Precision clock application API: local raw clock + optional NTP discipline,
 * UTC/local/custom offset formatting, calibration, CPU affinity and overhead
 * accounting.  These symbols are exported for static and dynamic libraries and
 * are intentionally usable from GUI, CLI and language wrappers.
 * -------------------------------------------------------------------------- */
#ifndef NANOCHRONO_PRECISION_CLOCK_API
#define NANOCHRONO_PRECISION_CLOCK_API 1

typedef enum nc_clock_view {
    NC_CLOCK_VIEW_STOPWATCH = 0,
    NC_CLOCK_VIEW_CLOCK = 1,
    NC_CLOCK_VIEW_TIMER = 2
} nc_clock_view_t;

typedef enum nc_clock_detail_mode {
    NC_CLOCK_DETAIL_SIMPLE = 0,
    NC_CLOCK_DETAIL_NANO = 1
} nc_clock_detail_mode_t;

typedef enum nc_time_zone_mode {
    NC_TIME_ZONE_LOCAL = 0,
    NC_TIME_ZONE_UTC = 1,
    NC_TIME_ZONE_CUSTOM_OFFSET = 2
} nc_time_zone_mode_t;

typedef struct nc_precision_clock_config {
    uint32_t view;
    uint32_t detail_mode;
    uint32_t zone_mode;
    int32_t utc_offset_minutes;
    uint32_t route;
    uint32_t cpu_index;
    uint32_t pin_cpu;
    uint32_t ntp_timeout_ms;
    char ntp_server[128];
} nc_precision_clock_config_t;

typedef struct nc_clock_calibration_result {
    int status;
    uint32_t route;
    uint32_t cpu_before;
    uint32_t cpu_after;
    uint32_t migrated;
    uint32_t pinned;
    uint64_t samples;
    uint64_t elapsed_raw_units;
    uint64_t elapsed_ns;
    double units_per_second;
    double ns_per_unit;
    double ppm_error_vs_context;
    uint64_t read_overhead_units;
    uint64_t kernel_timecall_overhead_cycles;
    uint64_t api_call_overhead_cycles;
} nc_clock_calibration_result_t;

typedef struct nc_ntp_sample {
    int status;
    char server[128];
    uint32_t route;
    uint32_t stratum;
    int32_t precision_exp;
    uint32_t leap_indicator;
    uint32_t version;
    uint32_t mode;
    uint32_t cpu_before;
    uint32_t cpu_after;
    uint32_t migrated;
    uint64_t local_send_unix_ns;
    uint64_t local_recv_unix_ns;
    uint64_t ntp_transmit_unix_ns;
    int64_t offset_ns;
    uint64_t delay_ns;
    uint64_t socket_setup_overhead_units;
    uint64_t send_recv_overhead_units;
    uint64_t kernel_timecall_overhead_cycles;
    uint64_t api_call_overhead_cycles;
    uint64_t raw_before_units;
    uint64_t raw_after_units;
} nc_ntp_sample_t;

NC_API const char *nc_clock_view_name(nc_clock_view_t view);
NC_API const char *nc_clock_detail_mode_name(nc_clock_detail_mode_t mode);
NC_API const char *nc_time_zone_mode_name(nc_time_zone_mode_t mode);
NC_API int nc_format_unix_time_ns_ex(uint64_t unix_ns, nc_time_zone_mode_t mode,
                                     int32_t utc_offset_minutes, char *buf, size_t cap);
NC_API int nc_precision_clock_default_config(nc_precision_clock_config_t *cfg);
NC_API int nc_clock_pin_thread_to_cpu(uint32_t cpu_index);
NC_API uint32_t nc_clock_current_cpu(void);
/* Runtime stability diagnostics for production-safe benchmark/profiler runs.
 * Exported by all libraries and usable from CLI, GUI and language wrappers. */
typedef struct nc_cpu_pin_validation {
    int status;
    uint32_t requested_cpu;
    uint32_t pinned;
    uint32_t cpu_before;
    uint32_t cpu_after;
    uint32_t migrated;
    uint32_t migrations;
    uint32_t samples;
    uint32_t first_migration_sample;
    uint64_t observed_cpu_mask_low;
} nc_cpu_pin_validation_t;

typedef struct nc_core_migration_result {
    int status;
    uint32_t cpu_start;
    uint32_t cpu_end;
    uint32_t migrated;
    uint32_t migrations;
    uint32_t samples;
    uint32_t first_migration_sample;
    uint64_t observed_cpu_mask_low;
} nc_core_migration_result_t;

typedef struct nc_frequency_drift_result {
    int status;
    uint32_t route;
    uint32_t samples;
    uint32_t interval_ms;
    uint32_t cpu_before;
    uint32_t cpu_after;
    uint32_t migrated;
    uint64_t raw_start;
    uint64_t raw_end;
    uint64_t wall_start_ns;
    uint64_t wall_end_ns;
    uint64_t elapsed_units;
    uint64_t elapsed_ns;
    double units_per_second;
    double ns_per_unit;
    double ppm_error_vs_context;
} nc_frequency_drift_result_t;

typedef struct nc_thermal_state {
    int status;
    int32_t temperature_millicelsius;
    uint32_t throttling_detected;
    uint32_t source_available;
    char source[96];
} nc_thermal_state_t;

typedef struct nc_system_stability_snapshot {
    int status;
    nc_cpu_pin_validation_t pin;
    nc_core_migration_result_t migration;
    nc_frequency_drift_result_t drift;
    nc_thermal_state_t thermal;
} nc_system_stability_snapshot_t;

NC_API int nc_clock_validate_cpu_pin(uint32_t cpu_index, uint32_t samples, uint32_t interval_us, nc_cpu_pin_validation_t *out);
NC_API int nc_clock_detect_core_migration(uint32_t samples, uint32_t interval_us, nc_core_migration_result_t *out);
NC_API int nc_clock_measure_frequency_drift(nc_ctx_t *ctx, nc_clock_route_t route, uint32_t samples, uint32_t interval_ms, nc_frequency_drift_result_t *out);
NC_API int nc_clock_query_thermal_state(nc_thermal_state_t *out);
NC_API int nc_clock_system_stability_snapshot(nc_ctx_t *ctx, uint32_t cpu_index, nc_system_stability_snapshot_t *out);

NC_API uint64_t nc_measure_kernel_timecall_overhead_cycles(nc_ctx_t *ctx, uint32_t iterations);
NC_API uint64_t nc_measure_api_call_overhead_cycles(nc_ctx_t *ctx, uint32_t iterations);
NC_API int nc_calibrate_clock_route(nc_ctx_t *ctx, nc_clock_route_t route,
                                    uint32_t samples, uint32_t pin_cpu,
                                    uint32_t cpu_index,
                                    nc_clock_calibration_result_t *out);
NC_API int nc_ntp_query(nc_ctx_t *ctx, const char *server, uint32_t timeout_ms,
                        nc_clock_route_t route, nc_ntp_sample_t *out);

/* Stable low-overhead cycle-to-nanosecond calibration for HFT/extreme bench. */
typedef struct nc_stable_clock_config {
    uint32_t route;
    uint32_t pin_cpu;
    uint32_t cpu_index;
    uint32_t warmup_ms;
    uint32_t calibration_ms;
    uint32_t samples;
    uint32_t require_no_migration;
} nc_stable_clock_config_t;

typedef struct nc_stable_clock_state {
    int status;
    uint32_t route;
    uint32_t pinned;
    uint32_t cpu_index;
    uint32_t cpu_before;
    uint32_t cpu_after;
    uint32_t migrated;
    uint32_t invariant_hint;
    uint64_t raw_start;
    uint64_t raw_end;
    uint64_t elapsed_units;
    uint64_t elapsed_ns;
    double cycles_per_ns;
    double ns_per_cycle;
    uint64_t read_overhead_units;
    uint64_t kernel_timecall_overhead_cycles;
    uint64_t api_call_overhead_cycles;
    int64_t ppm_error_vs_previous;
} nc_stable_clock_state_t;

NC_API int nc_stable_clock_default_config(nc_stable_clock_config_t *cfg);
NC_API int nc_stable_clock_prepare(const nc_stable_clock_config_t *cfg, nc_stable_clock_state_t *out);
NC_API int nc_calibrate_cycles_per_ns(nc_ctx_t *ctx, const nc_stable_clock_config_t *cfg, nc_stable_clock_state_t *out);
NC_API uint64_t nc_cycles_to_ns_calibrated(uint64_t cycles, double cycles_per_ns);
NC_API uint64_t nc_raw_delta_to_ns_calibrated(uint64_t raw_start, uint64_t raw_end, double units_per_ns);
NC_API uint64_t nc_clock_read_raw_route(nc_ctx_t *ctx, nc_clock_route_t route);
NC_API const char *nc_clock_stability_advice(void);

#endif /* NANOCHRONO_PRECISION_CLOCK_API */


#ifdef __cplusplus
}
#endif
#endif /* NANOCHRONO_TOOLKIT_API */
