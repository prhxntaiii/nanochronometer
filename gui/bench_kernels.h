#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nc_bench_kernel_id {
    NC_BENCH_NONE = 0,
    NC_BENCH_ASM_LEGACY,
    NC_BENCH_MMX,
    NC_BENCH_SSE,
    NC_BENCH_SSE2,
    NC_BENCH_SSE3,
    NC_BENCH_SSSE3,
    NC_BENCH_SSE41,
    NC_BENCH_SSE42,
    NC_BENCH_AESNI,
    NC_BENCH_PCLMULQDQ,
    NC_BENCH_SHANI,
    NC_BENCH_AVX,
    NC_BENCH_F16C,
    NC_BENCH_FMA,
    NC_BENCH_AVX2,
    NC_BENCH_AVX_VNNI,
    NC_BENCH_VAES,
    NC_BENCH_AVX512F,
    NC_BENCH_AVX512BW,
    NC_BENCH_AVX512VL,
    NC_BENCH_AVX512VNNI,
    NC_BENCH_COUNT
} nc_bench_kernel_id_t;

typedef struct nc_bench_cpu_features {
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
} nc_bench_cpu_features_t;

void     nc_bench_get_cpu_features(nc_bench_cpu_features_t *out);
int      nc_bench_kernel_available(nc_bench_kernel_id_t id, const nc_bench_cpu_features_t *features);
const char *nc_bench_kernel_name(nc_bench_kernel_id_t id);
const char *nc_bench_kernel_short_name(nc_bench_kernel_id_t id);
uint64_t nc_bench_run_cpu_kernel(nc_bench_kernel_id_t id, size_t loops);

uint64_t nc_bench_kernel_asm_legacy(size_t loops);
uint64_t nc_bench_kernel_mmx(size_t loops);
uint64_t nc_bench_kernel_sse(size_t loops);
uint64_t nc_bench_kernel_sse2(size_t loops);
uint64_t nc_bench_kernel_sse3(size_t loops);
uint64_t nc_bench_kernel_ssse3(size_t loops);
uint64_t nc_bench_kernel_sse41(size_t loops);
uint64_t nc_bench_kernel_sse42(size_t loops);
uint64_t nc_bench_kernel_aesni(size_t loops);
uint64_t nc_bench_kernel_pclmul(size_t loops);
uint64_t nc_bench_kernel_shani(size_t loops);
uint64_t nc_bench_kernel_avx(size_t loops);
uint64_t nc_bench_kernel_f16c(size_t loops);
uint64_t nc_bench_kernel_fma(size_t loops);
uint64_t nc_bench_kernel_avx2(size_t loops);
uint64_t nc_bench_kernel_avx_vnni(size_t loops);
uint64_t nc_bench_kernel_vaes(size_t loops);
uint64_t nc_bench_kernel_avx512(size_t loops);
uint64_t nc_bench_kernel_avx512bw(size_t loops);
uint64_t nc_bench_kernel_avx512vl(size_t loops);
uint64_t nc_bench_kernel_avx512vnni(size_t loops);

#ifdef __cplusplus
}
#endif
