#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "nanochrono.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <sched.h>
#  if defined(__linux__)
#    include <pthread.h>
#  endif
#  if defined(__ANDROID__)
#    include <sys/syscall.h>
#    include <asm/unistd.h>
#  endif
#  if defined(__linux__)
#    include <dirent.h>
#  endif
#endif
#include <string.h>
#include <math.h>
#if defined(__linux__) && defined(__aarch64__)
#include <sys/auxv.h>
#include <asm/hwcap.h>
#endif

#ifndef NC_ARRAY_COUNT
#define NC_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#endif

static uint32_t nc_lcg32_(uint32_t *s) {
    uint32_t x = *s ? *s : 0x12345678u;
    x = x * 1664525u + 1013904223u;
    *s = x;
    return x;
}

static uint64_t nc_abs_delta_u64_(uint64_t a, uint64_t b) { return a > b ? a - b : b - a; }
#if defined(__x86_64__) || defined(_M_X64)
extern uint64_t nc_x64_mb_sisd_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sisd_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtsc_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_rdtscp_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_lfence_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_mfence_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_prefetch_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_cache_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_sse2_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_aesni_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_pclmul_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_shani_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_x64_mb_avx2_024(uint32_t iterations, void *buffer, size_t bytes);
#elif defined(__aarch64__) || defined(_M_ARM64)
extern uint64_t nc_arm64_mb_sisd_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_sisd_049(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_counter_049(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_barrier_049(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_memory_049(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_neon_049(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_000(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_001(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_002(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_003(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_004(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_005(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_006(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_007(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_008(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_009(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_010(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_011(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_012(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_013(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_014(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_015(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_016(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_017(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_018(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_019(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_020(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_021(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_022(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_023(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_024(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_025(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_026(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_027(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_028(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_029(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_030(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_031(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_032(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_033(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_034(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_035(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_036(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_037(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_038(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_039(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_040(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_041(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_042(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_043(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_044(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_045(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_046(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_047(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_048(uint32_t iterations, void *buffer, size_t bytes);
extern uint64_t nc_arm64_mb_branch_049(uint32_t iterations, void *buffer, size_t bytes);
#endif

static const nc_microbench_descriptor_t g_nc_microbench_catalog[] = {
#if defined(__x86_64__) || defined(_M_X64)
    {"x64.sisd.000", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_000},
    {"x64.sisd.001", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_001},
    {"x64.sisd.002", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_002},
    {"x64.sisd.003", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_003},
    {"x64.sisd.004", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_004},
    {"x64.sisd.005", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_005},
    {"x64.sisd.006", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_006},
    {"x64.sisd.007", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_007},
    {"x64.sisd.008", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_008},
    {"x64.sisd.009", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_009},
    {"x64.sisd.010", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_010},
    {"x64.sisd.011", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_011},
    {"x64.sisd.012", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_012},
    {"x64.sisd.013", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_013},
    {"x64.sisd.014", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_014},
    {"x64.sisd.015", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_015},
    {"x64.sisd.016", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_016},
    {"x64.sisd.017", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_017},
    {"x64.sisd.018", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_018},
    {"x64.sisd.019", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_019},
    {"x64.sisd.020", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_020},
    {"x64.sisd.021", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_021},
    {"x64.sisd.022", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_022},
    {"x64.sisd.023", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_023},
    {"x64.sisd.024", "x64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_sisd_024},
    {"x64.rdtsc.000", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_000},
    {"x64.rdtsc.001", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_001},
    {"x64.rdtsc.002", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_002},
    {"x64.rdtsc.003", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_003},
    {"x64.rdtsc.004", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_004},
    {"x64.rdtsc.005", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_005},
    {"x64.rdtsc.006", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_006},
    {"x64.rdtsc.007", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_007},
    {"x64.rdtsc.008", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_008},
    {"x64.rdtsc.009", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_009},
    {"x64.rdtsc.010", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_010},
    {"x64.rdtsc.011", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_011},
    {"x64.rdtsc.012", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_012},
    {"x64.rdtsc.013", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_013},
    {"x64.rdtsc.014", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_014},
    {"x64.rdtsc.015", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_015},
    {"x64.rdtsc.016", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_016},
    {"x64.rdtsc.017", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_017},
    {"x64.rdtsc.018", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_018},
    {"x64.rdtsc.019", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_019},
    {"x64.rdtsc.020", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_020},
    {"x64.rdtsc.021", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_021},
    {"x64.rdtsc.022", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_022},
    {"x64.rdtsc.023", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_023},
    {"x64.rdtsc.024", "x64", "rdtsc", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtsc_024},
    {"x64.rdtscp.000", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_000},
    {"x64.rdtscp.001", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_001},
    {"x64.rdtscp.002", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_002},
    {"x64.rdtscp.003", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_003},
    {"x64.rdtscp.004", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_004},
    {"x64.rdtscp.005", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_005},
    {"x64.rdtscp.006", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_006},
    {"x64.rdtscp.007", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_007},
    {"x64.rdtscp.008", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_008},
    {"x64.rdtscp.009", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_009},
    {"x64.rdtscp.010", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_010},
    {"x64.rdtscp.011", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_011},
    {"x64.rdtscp.012", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_012},
    {"x64.rdtscp.013", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_013},
    {"x64.rdtscp.014", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_014},
    {"x64.rdtscp.015", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_015},
    {"x64.rdtscp.016", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_016},
    {"x64.rdtscp.017", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_017},
    {"x64.rdtscp.018", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_018},
    {"x64.rdtscp.019", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_019},
    {"x64.rdtscp.020", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_020},
    {"x64.rdtscp.021", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_021},
    {"x64.rdtscp.022", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_022},
    {"x64.rdtscp.023", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_023},
    {"x64.rdtscp.024", "x64", "rdtscp", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_rdtscp_024},
    {"x64.lfence.000", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_000},
    {"x64.lfence.001", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_001},
    {"x64.lfence.002", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_002},
    {"x64.lfence.003", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_003},
    {"x64.lfence.004", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_004},
    {"x64.lfence.005", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_005},
    {"x64.lfence.006", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_006},
    {"x64.lfence.007", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_007},
    {"x64.lfence.008", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_008},
    {"x64.lfence.009", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_009},
    {"x64.lfence.010", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_010},
    {"x64.lfence.011", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_011},
    {"x64.lfence.012", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_012},
    {"x64.lfence.013", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_013},
    {"x64.lfence.014", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_014},
    {"x64.lfence.015", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_015},
    {"x64.lfence.016", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_016},
    {"x64.lfence.017", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_017},
    {"x64.lfence.018", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_018},
    {"x64.lfence.019", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_019},
    {"x64.lfence.020", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_020},
    {"x64.lfence.021", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_021},
    {"x64.lfence.022", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_022},
    {"x64.lfence.023", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_023},
    {"x64.lfence.024", "x64", "lfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_lfence_024},
    {"x64.mfence.000", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_000},
    {"x64.mfence.001", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_001},
    {"x64.mfence.002", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_002},
    {"x64.mfence.003", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_003},
    {"x64.mfence.004", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_004},
    {"x64.mfence.005", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_005},
    {"x64.mfence.006", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_006},
    {"x64.mfence.007", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_007},
    {"x64.mfence.008", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_008},
    {"x64.mfence.009", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_009},
    {"x64.mfence.010", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_010},
    {"x64.mfence.011", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_011},
    {"x64.mfence.012", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_012},
    {"x64.mfence.013", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_013},
    {"x64.mfence.014", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_014},
    {"x64.mfence.015", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_015},
    {"x64.mfence.016", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_016},
    {"x64.mfence.017", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_017},
    {"x64.mfence.018", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_018},
    {"x64.mfence.019", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_019},
    {"x64.mfence.020", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_020},
    {"x64.mfence.021", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_021},
    {"x64.mfence.022", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_022},
    {"x64.mfence.023", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_023},
    {"x64.mfence.024", "x64", "mfence", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_mfence_024},
    {"x64.prefetch.000", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_000},
    {"x64.prefetch.001", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_001},
    {"x64.prefetch.002", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_002},
    {"x64.prefetch.003", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_003},
    {"x64.prefetch.004", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_004},
    {"x64.prefetch.005", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_005},
    {"x64.prefetch.006", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_006},
    {"x64.prefetch.007", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_007},
    {"x64.prefetch.008", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_008},
    {"x64.prefetch.009", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_009},
    {"x64.prefetch.010", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_010},
    {"x64.prefetch.011", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_011},
    {"x64.prefetch.012", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_012},
    {"x64.prefetch.013", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_013},
    {"x64.prefetch.014", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_014},
    {"x64.prefetch.015", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_015},
    {"x64.prefetch.016", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_016},
    {"x64.prefetch.017", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_017},
    {"x64.prefetch.018", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_018},
    {"x64.prefetch.019", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_019},
    {"x64.prefetch.020", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_020},
    {"x64.prefetch.021", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_021},
    {"x64.prefetch.022", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_022},
    {"x64.prefetch.023", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_023},
    {"x64.prefetch.024", "x64", "prefetch", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_prefetch_024},
    {"x64.cache.000", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_000},
    {"x64.cache.001", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_001},
    {"x64.cache.002", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_002},
    {"x64.cache.003", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_003},
    {"x64.cache.004", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_004},
    {"x64.cache.005", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_005},
    {"x64.cache.006", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_006},
    {"x64.cache.007", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_007},
    {"x64.cache.008", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_008},
    {"x64.cache.009", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_009},
    {"x64.cache.010", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_010},
    {"x64.cache.011", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_011},
    {"x64.cache.012", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_012},
    {"x64.cache.013", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_013},
    {"x64.cache.014", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_014},
    {"x64.cache.015", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_015},
    {"x64.cache.016", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_016},
    {"x64.cache.017", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_017},
    {"x64.cache.018", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_018},
    {"x64.cache.019", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_019},
    {"x64.cache.020", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_020},
    {"x64.cache.021", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_021},
    {"x64.cache.022", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_022},
    {"x64.cache.023", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_023},
    {"x64.cache.024", "x64", "cache", "cache", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_x64_mb_cache_024},
    {"x64.sse2.000", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_000},
    {"x64.sse2.001", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_001},
    {"x64.sse2.002", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_002},
    {"x64.sse2.003", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_003},
    {"x64.sse2.004", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_004},
    {"x64.sse2.005", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_005},
    {"x64.sse2.006", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_006},
    {"x64.sse2.007", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_007},
    {"x64.sse2.008", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_008},
    {"x64.sse2.009", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_009},
    {"x64.sse2.010", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_010},
    {"x64.sse2.011", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_011},
    {"x64.sse2.012", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_012},
    {"x64.sse2.013", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_013},
    {"x64.sse2.014", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_014},
    {"x64.sse2.015", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_015},
    {"x64.sse2.016", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_016},
    {"x64.sse2.017", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_017},
    {"x64.sse2.018", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_018},
    {"x64.sse2.019", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_019},
    {"x64.sse2.020", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_020},
    {"x64.sse2.021", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_021},
    {"x64.sse2.022", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_022},
    {"x64.sse2.023", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_023},
    {"x64.sse2.024", "x64", "sse2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SSE2, nc_x64_mb_sse2_024},
    {"x64.aesni.000", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_000},
    {"x64.aesni.001", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_001},
    {"x64.aesni.002", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_002},
    {"x64.aesni.003", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_003},
    {"x64.aesni.004", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_004},
    {"x64.aesni.005", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_005},
    {"x64.aesni.006", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_006},
    {"x64.aesni.007", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_007},
    {"x64.aesni.008", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_008},
    {"x64.aesni.009", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_009},
    {"x64.aesni.010", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_010},
    {"x64.aesni.011", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_011},
    {"x64.aesni.012", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_012},
    {"x64.aesni.013", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_013},
    {"x64.aesni.014", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_014},
    {"x64.aesni.015", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_015},
    {"x64.aesni.016", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_016},
    {"x64.aesni.017", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_017},
    {"x64.aesni.018", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_018},
    {"x64.aesni.019", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_019},
    {"x64.aesni.020", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_020},
    {"x64.aesni.021", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_021},
    {"x64.aesni.022", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_022},
    {"x64.aesni.023", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_023},
    {"x64.aesni.024", "x64", "aesni", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AESNI, nc_x64_mb_aesni_024},
    {"x64.pclmul.000", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_000},
    {"x64.pclmul.001", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_001},
    {"x64.pclmul.002", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_002},
    {"x64.pclmul.003", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_003},
    {"x64.pclmul.004", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_004},
    {"x64.pclmul.005", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_005},
    {"x64.pclmul.006", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_006},
    {"x64.pclmul.007", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_007},
    {"x64.pclmul.008", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_008},
    {"x64.pclmul.009", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_009},
    {"x64.pclmul.010", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_010},
    {"x64.pclmul.011", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_011},
    {"x64.pclmul.012", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_012},
    {"x64.pclmul.013", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_013},
    {"x64.pclmul.014", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_014},
    {"x64.pclmul.015", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_015},
    {"x64.pclmul.016", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_016},
    {"x64.pclmul.017", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_017},
    {"x64.pclmul.018", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_018},
    {"x64.pclmul.019", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_019},
    {"x64.pclmul.020", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_020},
    {"x64.pclmul.021", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_021},
    {"x64.pclmul.022", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_022},
    {"x64.pclmul.023", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_023},
    {"x64.pclmul.024", "x64", "pclmul", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_PCLMUL, nc_x64_mb_pclmul_024},
    {"x64.shani.000", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_000},
    {"x64.shani.001", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_001},
    {"x64.shani.002", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_002},
    {"x64.shani.003", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_003},
    {"x64.shani.004", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_004},
    {"x64.shani.005", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_005},
    {"x64.shani.006", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_006},
    {"x64.shani.007", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_007},
    {"x64.shani.008", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_008},
    {"x64.shani.009", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_009},
    {"x64.shani.010", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_010},
    {"x64.shani.011", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_011},
    {"x64.shani.012", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_012},
    {"x64.shani.013", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_013},
    {"x64.shani.014", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_014},
    {"x64.shani.015", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_015},
    {"x64.shani.016", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_016},
    {"x64.shani.017", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_017},
    {"x64.shani.018", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_018},
    {"x64.shani.019", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_019},
    {"x64.shani.020", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_020},
    {"x64.shani.021", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_021},
    {"x64.shani.022", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_022},
    {"x64.shani.023", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_023},
    {"x64.shani.024", "x64", "shani", "crypto-isa", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_SHANI, nc_x64_mb_shani_024},
    {"x64.avx2.000", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_000},
    {"x64.avx2.001", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_001},
    {"x64.avx2.002", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_002},
    {"x64.avx2.003", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_003},
    {"x64.avx2.004", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_004},
    {"x64.avx2.005", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_005},
    {"x64.avx2.006", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_006},
    {"x64.avx2.007", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_007},
    {"x64.avx2.008", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_008},
    {"x64.avx2.009", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_009},
    {"x64.avx2.010", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_010},
    {"x64.avx2.011", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_011},
    {"x64.avx2.012", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_012},
    {"x64.avx2.013", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_013},
    {"x64.avx2.014", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_014},
    {"x64.avx2.015", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_015},
    {"x64.avx2.016", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_016},
    {"x64.avx2.017", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_017},
    {"x64.avx2.018", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_018},
    {"x64.avx2.019", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_019},
    {"x64.avx2.020", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_020},
    {"x64.avx2.021", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_021},
    {"x64.avx2.022", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_022},
    {"x64.avx2.023", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_023},
    {"x64.avx2.024", "x64", "avx2", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_AVX2, nc_x64_mb_avx2_024},
#elif defined(__aarch64__) || defined(_M_ARM64)
    {"arm64.sisd.000", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_000},
    {"arm64.sisd.001", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_001},
    {"arm64.sisd.002", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_002},
    {"arm64.sisd.003", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_003},
    {"arm64.sisd.004", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_004},
    {"arm64.sisd.005", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_005},
    {"arm64.sisd.006", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_006},
    {"arm64.sisd.007", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_007},
    {"arm64.sisd.008", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_008},
    {"arm64.sisd.009", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_009},
    {"arm64.sisd.010", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_010},
    {"arm64.sisd.011", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_011},
    {"arm64.sisd.012", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_012},
    {"arm64.sisd.013", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_013},
    {"arm64.sisd.014", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_014},
    {"arm64.sisd.015", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_015},
    {"arm64.sisd.016", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_016},
    {"arm64.sisd.017", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_017},
    {"arm64.sisd.018", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_018},
    {"arm64.sisd.019", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_019},
    {"arm64.sisd.020", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_020},
    {"arm64.sisd.021", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_021},
    {"arm64.sisd.022", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_022},
    {"arm64.sisd.023", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_023},
    {"arm64.sisd.024", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_024},
    {"arm64.sisd.025", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_025},
    {"arm64.sisd.026", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_026},
    {"arm64.sisd.027", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_027},
    {"arm64.sisd.028", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_028},
    {"arm64.sisd.029", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_029},
    {"arm64.sisd.030", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_030},
    {"arm64.sisd.031", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_031},
    {"arm64.sisd.032", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_032},
    {"arm64.sisd.033", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_033},
    {"arm64.sisd.034", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_034},
    {"arm64.sisd.035", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_035},
    {"arm64.sisd.036", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_036},
    {"arm64.sisd.037", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_037},
    {"arm64.sisd.038", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_038},
    {"arm64.sisd.039", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_039},
    {"arm64.sisd.040", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_040},
    {"arm64.sisd.041", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_041},
    {"arm64.sisd.042", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_042},
    {"arm64.sisd.043", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_043},
    {"arm64.sisd.044", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_044},
    {"arm64.sisd.045", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_045},
    {"arm64.sisd.046", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_046},
    {"arm64.sisd.047", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_047},
    {"arm64.sisd.048", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_048},
    {"arm64.sisd.049", "arm64", "sisd", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_sisd_049},
    {"arm64.counter.000", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_000},
    {"arm64.counter.001", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_001},
    {"arm64.counter.002", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_002},
    {"arm64.counter.003", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_003},
    {"arm64.counter.004", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_004},
    {"arm64.counter.005", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_005},
    {"arm64.counter.006", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_006},
    {"arm64.counter.007", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_007},
    {"arm64.counter.008", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_008},
    {"arm64.counter.009", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_009},
    {"arm64.counter.010", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_010},
    {"arm64.counter.011", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_011},
    {"arm64.counter.012", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_012},
    {"arm64.counter.013", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_013},
    {"arm64.counter.014", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_014},
    {"arm64.counter.015", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_015},
    {"arm64.counter.016", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_016},
    {"arm64.counter.017", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_017},
    {"arm64.counter.018", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_018},
    {"arm64.counter.019", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_019},
    {"arm64.counter.020", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_020},
    {"arm64.counter.021", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_021},
    {"arm64.counter.022", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_022},
    {"arm64.counter.023", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_023},
    {"arm64.counter.024", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_024},
    {"arm64.counter.025", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_025},
    {"arm64.counter.026", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_026},
    {"arm64.counter.027", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_027},
    {"arm64.counter.028", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_028},
    {"arm64.counter.029", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_029},
    {"arm64.counter.030", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_030},
    {"arm64.counter.031", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_031},
    {"arm64.counter.032", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_032},
    {"arm64.counter.033", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_033},
    {"arm64.counter.034", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_034},
    {"arm64.counter.035", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_035},
    {"arm64.counter.036", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_036},
    {"arm64.counter.037", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_037},
    {"arm64.counter.038", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_038},
    {"arm64.counter.039", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_039},
    {"arm64.counter.040", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_040},
    {"arm64.counter.041", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_041},
    {"arm64.counter.042", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_042},
    {"arm64.counter.043", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_043},
    {"arm64.counter.044", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_044},
    {"arm64.counter.045", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_045},
    {"arm64.counter.046", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_046},
    {"arm64.counter.047", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_047},
    {"arm64.counter.048", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_048},
    {"arm64.counter.049", "arm64", "counter", "timer", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_counter_049},
    {"arm64.barrier.000", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_000},
    {"arm64.barrier.001", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_001},
    {"arm64.barrier.002", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_002},
    {"arm64.barrier.003", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_003},
    {"arm64.barrier.004", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_004},
    {"arm64.barrier.005", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_005},
    {"arm64.barrier.006", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_006},
    {"arm64.barrier.007", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_007},
    {"arm64.barrier.008", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_008},
    {"arm64.barrier.009", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_009},
    {"arm64.barrier.010", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_010},
    {"arm64.barrier.011", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_011},
    {"arm64.barrier.012", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_012},
    {"arm64.barrier.013", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_013},
    {"arm64.barrier.014", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_014},
    {"arm64.barrier.015", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_015},
    {"arm64.barrier.016", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_016},
    {"arm64.barrier.017", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_017},
    {"arm64.barrier.018", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_018},
    {"arm64.barrier.019", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_019},
    {"arm64.barrier.020", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_020},
    {"arm64.barrier.021", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_021},
    {"arm64.barrier.022", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_022},
    {"arm64.barrier.023", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_023},
    {"arm64.barrier.024", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_024},
    {"arm64.barrier.025", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_025},
    {"arm64.barrier.026", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_026},
    {"arm64.barrier.027", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_027},
    {"arm64.barrier.028", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_028},
    {"arm64.barrier.029", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_029},
    {"arm64.barrier.030", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_030},
    {"arm64.barrier.031", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_031},
    {"arm64.barrier.032", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_032},
    {"arm64.barrier.033", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_033},
    {"arm64.barrier.034", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_034},
    {"arm64.barrier.035", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_035},
    {"arm64.barrier.036", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_036},
    {"arm64.barrier.037", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_037},
    {"arm64.barrier.038", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_038},
    {"arm64.barrier.039", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_039},
    {"arm64.barrier.040", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_040},
    {"arm64.barrier.041", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_041},
    {"arm64.barrier.042", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_042},
    {"arm64.barrier.043", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_043},
    {"arm64.barrier.044", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_044},
    {"arm64.barrier.045", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_045},
    {"arm64.barrier.046", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_046},
    {"arm64.barrier.047", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_047},
    {"arm64.barrier.048", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_048},
    {"arm64.barrier.049", "arm64", "barrier", "barrier", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_barrier_049},
    {"arm64.memory.000", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_000},
    {"arm64.memory.001", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_001},
    {"arm64.memory.002", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_002},
    {"arm64.memory.003", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_003},
    {"arm64.memory.004", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_004},
    {"arm64.memory.005", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_005},
    {"arm64.memory.006", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_006},
    {"arm64.memory.007", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_007},
    {"arm64.memory.008", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_008},
    {"arm64.memory.009", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_009},
    {"arm64.memory.010", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_010},
    {"arm64.memory.011", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_011},
    {"arm64.memory.012", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_012},
    {"arm64.memory.013", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_013},
    {"arm64.memory.014", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_014},
    {"arm64.memory.015", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_015},
    {"arm64.memory.016", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_016},
    {"arm64.memory.017", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_017},
    {"arm64.memory.018", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_018},
    {"arm64.memory.019", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_019},
    {"arm64.memory.020", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_020},
    {"arm64.memory.021", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_021},
    {"arm64.memory.022", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_022},
    {"arm64.memory.023", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_023},
    {"arm64.memory.024", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_024},
    {"arm64.memory.025", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_025},
    {"arm64.memory.026", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_026},
    {"arm64.memory.027", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_027},
    {"arm64.memory.028", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_028},
    {"arm64.memory.029", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_029},
    {"arm64.memory.030", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_030},
    {"arm64.memory.031", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_031},
    {"arm64.memory.032", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_032},
    {"arm64.memory.033", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_033},
    {"arm64.memory.034", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_034},
    {"arm64.memory.035", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_035},
    {"arm64.memory.036", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_036},
    {"arm64.memory.037", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_037},
    {"arm64.memory.038", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_038},
    {"arm64.memory.039", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_039},
    {"arm64.memory.040", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_040},
    {"arm64.memory.041", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_041},
    {"arm64.memory.042", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_042},
    {"arm64.memory.043", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_043},
    {"arm64.memory.044", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_044},
    {"arm64.memory.045", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_045},
    {"arm64.memory.046", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_046},
    {"arm64.memory.047", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_047},
    {"arm64.memory.048", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_048},
    {"arm64.memory.049", "arm64", "memory", "memory", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_memory_049},
    {"arm64.neon.000", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_000},
    {"arm64.neon.001", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_001},
    {"arm64.neon.002", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_002},
    {"arm64.neon.003", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_003},
    {"arm64.neon.004", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_004},
    {"arm64.neon.005", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_005},
    {"arm64.neon.006", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_006},
    {"arm64.neon.007", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_007},
    {"arm64.neon.008", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_008},
    {"arm64.neon.009", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_009},
    {"arm64.neon.010", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_010},
    {"arm64.neon.011", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_011},
    {"arm64.neon.012", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_012},
    {"arm64.neon.013", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_013},
    {"arm64.neon.014", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_014},
    {"arm64.neon.015", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_015},
    {"arm64.neon.016", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_016},
    {"arm64.neon.017", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_017},
    {"arm64.neon.018", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_018},
    {"arm64.neon.019", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_019},
    {"arm64.neon.020", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_020},
    {"arm64.neon.021", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_021},
    {"arm64.neon.022", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_022},
    {"arm64.neon.023", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_023},
    {"arm64.neon.024", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_024},
    {"arm64.neon.025", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_025},
    {"arm64.neon.026", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_026},
    {"arm64.neon.027", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_027},
    {"arm64.neon.028", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_028},
    {"arm64.neon.029", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_029},
    {"arm64.neon.030", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_030},
    {"arm64.neon.031", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_031},
    {"arm64.neon.032", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_032},
    {"arm64.neon.033", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_033},
    {"arm64.neon.034", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_034},
    {"arm64.neon.035", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_035},
    {"arm64.neon.036", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_036},
    {"arm64.neon.037", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_037},
    {"arm64.neon.038", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_038},
    {"arm64.neon.039", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_039},
    {"arm64.neon.040", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_040},
    {"arm64.neon.041", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_041},
    {"arm64.neon.042", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_042},
    {"arm64.neon.043", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_043},
    {"arm64.neon.044", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_044},
    {"arm64.neon.045", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_045},
    {"arm64.neon.046", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_046},
    {"arm64.neon.047", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_047},
    {"arm64.neon.048", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_048},
    {"arm64.neon.049", "arm64", "neon", "simd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NEON, nc_arm64_mb_neon_049},
    {"arm64.branch.000", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_000},
    {"arm64.branch.001", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_001},
    {"arm64.branch.002", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_002},
    {"arm64.branch.003", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_003},
    {"arm64.branch.004", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_004},
    {"arm64.branch.005", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_005},
    {"arm64.branch.006", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_006},
    {"arm64.branch.007", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_007},
    {"arm64.branch.008", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_008},
    {"arm64.branch.009", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_009},
    {"arm64.branch.010", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_010},
    {"arm64.branch.011", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_011},
    {"arm64.branch.012", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_012},
    {"arm64.branch.013", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_013},
    {"arm64.branch.014", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_014},
    {"arm64.branch.015", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_015},
    {"arm64.branch.016", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_016},
    {"arm64.branch.017", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_017},
    {"arm64.branch.018", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_018},
    {"arm64.branch.019", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_019},
    {"arm64.branch.020", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_020},
    {"arm64.branch.021", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_021},
    {"arm64.branch.022", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_022},
    {"arm64.branch.023", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_023},
    {"arm64.branch.024", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_024},
    {"arm64.branch.025", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_025},
    {"arm64.branch.026", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_026},
    {"arm64.branch.027", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_027},
    {"arm64.branch.028", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_028},
    {"arm64.branch.029", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_029},
    {"arm64.branch.030", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_030},
    {"arm64.branch.031", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_031},
    {"arm64.branch.032", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_032},
    {"arm64.branch.033", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_033},
    {"arm64.branch.034", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_034},
    {"arm64.branch.035", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_035},
    {"arm64.branch.036", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_036},
    {"arm64.branch.037", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_037},
    {"arm64.branch.038", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_038},
    {"arm64.branch.039", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_039},
    {"arm64.branch.040", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_040},
    {"arm64.branch.041", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_041},
    {"arm64.branch.042", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_042},
    {"arm64.branch.043", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_043},
    {"arm64.branch.044", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_044},
    {"arm64.branch.045", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_045},
    {"arm64.branch.046", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_046},
    {"arm64.branch.047", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_047},
    {"arm64.branch.048", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_048},
    {"arm64.branch.049", "arm64", "branch", "sisd", NC_BENCHMARK_WHITE_BOX_ASM, NC_MICROBENCH_F_NONE, nc_arm64_mb_branch_049},
#endif
};


NC_API const char *nc_benchmark_mode_name(nc_benchmark_mode_t mode) {
    switch (mode) {
    case NC_BENCHMARK_BLACK_BOX_CRYPTO: return "black-box-crypto";
    case NC_BENCHMARK_GRAY_BOX_INTERNAL: return "gray-box-internal";
    case NC_BENCHMARK_WHITE_BOX_ASM: return "white-box-asm";
    default: return "unknown";
    }
}

NC_API size_t nc_microbench_catalog_count(void) {
    return NC_ARRAY_COUNT(g_nc_microbench_catalog);
}

NC_API const nc_microbench_descriptor_t *nc_microbench_catalog_entry(size_t index) {
    return index < nc_microbench_catalog_count() ? &g_nc_microbench_catalog[index] : NULL;
}

NC_API int nc_microbench_available(const nc_microbench_descriptor_t *d) {
    if (!d || !d->fn) return 0;
#if defined(__x86_64__) || defined(_M_X64)
    if ((d->feature_flags & NC_MICROBENCH_F_SSE2) && !nc_cpu_has_sse2()) return 0;
    if ((d->feature_flags & NC_MICROBENCH_F_AESNI) && !nc_cpu_has_aesni()) return 0;
    if ((d->feature_flags & NC_MICROBENCH_F_PCLMUL) && !nc_cpu_has_pclmulqdq()) return 0;
    if ((d->feature_flags & NC_MICROBENCH_F_SHANI) && !nc_cpu_has_shani()) return 0;
    if ((d->feature_flags & NC_MICROBENCH_F_AVX2) && !nc_cpu_has_avx2()) return 0;
    if ((d->feature_flags & NC_MICROBENCH_F_AVX512F) && !nc_cpu_has_avx512f()) return 0;
#elif defined(__aarch64__) || defined(_M_ARM64)
    (void)d;
#endif
    return 1;
}

NC_API uint64_t nc_microbench_run(nc_ctx_t *ctx, size_t index, uint32_t iterations,
                                  void *buffer, size_t bytes, nc_microbench_result_t *out) {
    const nc_microbench_descriptor_t *d = nc_microbench_catalog_entry(index);
    if (out) memset(out, 0, sizeof(*out));
    if (!d || !iterations) {
        if (out) out->status = NC_ERR_BAD_ARGUMENT;
        return 0;
    }
    if (!nc_microbench_available(d)) {
        if (out) out->status = NC_ERR_UNSUPPORTED;
        return 0;
    }
    uint64_t s = nc_now_cycles(ctx);
    uint64_t chk = d->fn(iterations, buffer, bytes);
    uint64_t e = nc_now_cycles(ctx);
    uint64_t cyc = e >= s ? e - s : 0;
    if (out) {
        out->status = NC_OK;
        out->cycles = cyc;
        out->ns = nc_cycles_to_ns(ctx, cyc);
        out->checksum = chk;
        out->iterations = iterations;
        out->catalog_index = (uint32_t)index;
    }
    return cyc;
}

NC_API uint64_t nc_benchmark_mode_cycles(nc_ctx_t *ctx, nc_benchmark_mode_t mode,
                                         nc_instruction_family_t family,
                                         uint32_t iterations, const void *input,
                                         size_t input_len, nc_instruction_result_t *out) {
    if (!iterations) iterations = 1024;
    if (!input_len) input_len = 4096;
    switch (mode) {
    case NC_BENCHMARK_BLACK_BOX_CRYPTO: {
        uint8_t digest[64];
        uint8_t *buf = (uint8_t*)malloc(input_len);
        if (!buf) { if (out) memset(out, 0, sizeof(*out)); return 0; }
        if (input) memcpy(buf, input, input_len); else memset(buf, 0xA5, input_len);
        uint64_t start = nc_now_cycles(ctx);
        uint64_t chk = 0;
        for (uint32_t i = 0; i < iterations; ++i) {
            if (family == NC_INSTR_SHA) chk ^= nc_crypto_sha256_cycles(ctx, buf, input_len, digest, NULL);
            else chk ^= nc_crypto_rand_cycles(ctx, digest, sizeof(digest), NULL);
        }
        uint64_t end = nc_now_cycles(ctx);
        uint64_t cyc = end >= start ? end - start : 0;
        if (out) { out->status = NC_OK; out->family = (uint32_t)family; out->backend = (uint32_t)nc_backend(ctx); out->cycles = cyc; out->ns = nc_cycles_to_ns(ctx, cyc); out->blocks = iterations; out->checksum = chk; }
        free(buf);
        return cyc;
    }
    case NC_BENCHMARK_GRAY_BOX_INTERNAL:
        if (family == NC_INSTR_AES) return nc_measure_aesenc_cycles(ctx, iterations, out);
        if (family == NC_INSTR_SHA) return nc_measure_sha256msg_cycles(ctx, iterations, out);
        if (family == NC_INSTR_PCLMULQDQ) return nc_measure_pclmul_cycles(ctx, iterations, out);
        return nc_measure_instruction_family_cycles(ctx, family, iterations, out);
    case NC_BENCHMARK_WHITE_BOX_ASM: {
        for (size_t i = 0; i < nc_microbench_catalog_count(); ++i) {
            const nc_microbench_descriptor_t *d = nc_microbench_catalog_entry(i);
            if (!d || d->mode != NC_BENCHMARK_WHITE_BOX_ASM) continue;
            if (family == NC_INSTR_AES && strcmp(d->family, "aesni") != 0) continue;
            if (family == NC_INSTR_SHA && strcmp(d->family, "shani") != 0) continue;
            if (family == NC_INSTR_PCLMULQDQ && strcmp(d->family, "pclmul") != 0) continue;
            if ((family == NC_INSTR_SSE2 || family == NC_INSTR_NEON) && strcmp(d->category, "simd") != 0) continue;
            if (family == NC_INSTR_SCALAR && strcmp(d->category, "sisd") != 0) continue;
            if (!nc_microbench_available(d)) continue;
            nc_microbench_result_t r;
            uint64_t cyc = nc_microbench_run(ctx, i, iterations, (void*)input, input_len, &r);
            if (out) { out->status = r.status; out->family = (uint32_t)family; out->backend = (uint32_t)nc_backend(ctx); out->cycles = r.cycles; out->ns = r.ns; out->blocks = iterations; out->checksum = r.checksum; }
            return cyc;
        }
        if (out) { memset(out, 0, sizeof(*out)); out->status = NC_ERR_UNSUPPORTED; out->family = (uint32_t)family; }
        return 0;
    }
    default:
        if (out) { memset(out, 0, sizeof(*out)); out->status = NC_ERR_BAD_ARGUMENT; }
        return 0;
    }
}

NC_API void nc_stopwatch_ns_reset(nc_stopwatch_ns_t *sw) {
    if (!sw) return;
    memset(sw, 0, sizeof(*sw));
}
NC_API void nc_stopwatch_ns_start(nc_stopwatch_ns_t *sw) {
    if (!sw) return;
    uint64_t now = nc_monotonic_time_ns();
    sw->start_ns = now;
    sw->last_lap_ns = now;
    sw->running = 1;
}
NC_API uint64_t nc_stopwatch_ns_lap(nc_stopwatch_ns_t *sw) {
    if (!sw || !sw->running) return 0;
    uint64_t now = nc_monotonic_time_ns();
    uint64_t lap = now >= sw->last_lap_ns ? now - sw->last_lap_ns : 0;
    sw->last_lap_ns = now;
    return lap;
}
NC_API uint64_t nc_stopwatch_ns_stop(nc_stopwatch_ns_t *sw) {
    if (!sw) return 0;
    if (sw->running) {
        uint64_t now = nc_monotonic_time_ns();
        sw->accumulated_ns += now >= sw->start_ns ? now - sw->start_ns : 0;
        sw->running = 0;
    }
    return sw->accumulated_ns;
}
NC_API uint64_t nc_stopwatch_ns_elapsed(const nc_stopwatch_ns_t *sw) {
    if (!sw) return 0;
    if (!sw->running) return sw->accumulated_ns;
    uint64_t now = nc_monotonic_time_ns();
    return sw->accumulated_ns + (now >= sw->start_ns ? now - sw->start_ns : 0);
}

NC_API int nc_sidechannel_audit_constant_time(nc_ctx_t *ctx,
                                              nc_sidechannel_candidate_t candidate,
                                              void *arg,
                                              const nc_sidechannel_audit_config_t *config,
                                              nc_sidechannel_audit_result_t *out) {
    if (!candidate || !out) return 0;
    nc_sidechannel_audit_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (config) cfg = *config;
    if (!cfg.samples) cfg.samples = 2000;
    if (cfg.samples < 16) cfg.samples = 16;
    if (!cfg.input_len) cfg.input_len = 64;
    if (!cfg.output_len) cfg.output_len = 64;
    if (!cfg.warmup) cfg.warmup = 64;
    uint8_t *in = (uint8_t*)malloc(cfg.input_len);
    uint8_t *outbuf = (uint8_t*)malloc(cfg.output_len);
    uint64_t *fixed = (uint64_t*)malloc((size_t)cfg.samples * sizeof(uint64_t));
    uint64_t *random = (uint64_t*)malloc((size_t)cfg.samples * sizeof(uint64_t));
    if (!in || !outbuf || !fixed || !random) {
        free(in); free(outbuf); free(fixed); free(random);
        memset(out, 0, sizeof(*out)); out->status = NC_ERR_BAD_ARGUMENT;
        return 0;
    }
    uint32_t seed = cfg.fixed_seed ? cfg.fixed_seed : 0xC0FFEEu;
    memset(in, cfg.fixed_value, cfg.input_len);
    for (uint32_t i = 0; i < cfg.warmup; ++i) candidate(outbuf, cfg.output_len, in, cfg.input_len, arg);
    for (uint32_t i = 0; i < cfg.samples; ++i) {
        memset(in, cfg.fixed_value, cfg.input_len);
        uint64_t s0 = nc_now_cycles(ctx);
        candidate(outbuf, cfg.output_len, in, cfg.input_len, arg);
        uint64_t e0 = nc_now_cycles(ctx);
        fixed[i] = e0 >= s0 ? e0 - s0 : 0;
        for (size_t j = 0; j < cfg.input_len; ++j) in[j] = (uint8_t)(nc_lcg32_(&seed) >> 24);
        uint64_t s1 = nc_now_cycles(ctx);
        candidate(outbuf, cfg.output_len, in, cfg.input_len, arg);
        uint64_t e1 = nc_now_cycles(ctx);
        random[i] = e1 >= s1 ? e1 - s1 : 0;
    }
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    out->samples = cfg.samples;
    nc_analyze_samples(fixed, cfg.samples, &out->fixed_stats);
    nc_analyze_samples(random, cfg.samples, &out->random_stats);
    out->welch_t = nc_welch_t_score(fixed, cfg.samples, random, cfg.samples);
    out->abs_welch_t = out->welch_t < 0.0 ? -out->welch_t : out->welch_t;
    out->likely_timing_leak = out->abs_welch_t >= 4.5 ? 1 : 0;
    out->mean_delta_cycles = nc_abs_delta_u64_(out->fixed_stats.mean_cycles, out->random_stats.mean_cycles);
    out->mean_delta_ns = nc_cycles_to_ns(ctx, out->mean_delta_cycles);
    free(in); free(outbuf); free(fixed); free(random);
    return 1;
}

NC_API uint64_t nc_measure_load_probe_asm_cycles(nc_ctx_t *ctx, const void *ptr, nc_sidechannel_result_t *out) {
    if (!ptr) { if (out) memset(out, 0, sizeof(*out)); return 0; }
    nc_asm_sidechannel_probe_result_t cached_r, flushed_r, prefetched_r;
    memset(&cached_r, 0, sizeof(cached_r));
    memset(&flushed_r, 0, sizeof(flushed_r));
    memset(&prefetched_r, 0, sizeof(prefetched_r));
    uint64_t cached = nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_LOAD_FENCED, (void*)ptr, 0, 0, &cached_r);
    uint64_t flushed = nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_FLUSH_RELOAD, (void*)ptr, 0, 0, &flushed_r);
    uint64_t prefetched = nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_PREFETCH_RELOAD, (void*)ptr, 0, 0, &prefetched_r);
    if (flushed_r.status == NC_ERR_UNSUPPORTED) {
        flushed = nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_LOAD, (void*)ptr, 0, 0, &flushed_r);
    }
    if (out) {
        out->status = NC_OK;
        out->cached_cycles = cached;
        out->flushed_cycles = flushed;
        out->prefetched_cycles = prefetched;
        out->threshold_cycles = (cached + flushed) / 2u;
        out->separation_score = (double)flushed - (double)cached;
    }
    return flushed;
}

/* -------------------------------------------------------------------------
 * Low-level assembly side-channel measurement wrappers.
 * These wrappers intentionally operate only on caller-provided local buffers.
 * ------------------------------------------------------------------------- */
NC_API const char *nc_asm_sidechannel_probe_name(nc_asm_sidechannel_probe_kind_t kind) {
    switch (kind) {
    case NC_ASM_SC_PROBE_LOAD: return "asm-load";
    case NC_ASM_SC_PROBE_LOAD_FENCED: return "asm-load-fenced";
    case NC_ASM_SC_PROBE_FLUSH_RELOAD: return "asm-flush-reload";
    case NC_ASM_SC_PROBE_PREFETCH_RELOAD: return "asm-prefetch-reload";
    case NC_ASM_SC_PROBE_STORE: return "asm-store";
    case NC_ASM_SC_PROBE_BRANCH: return "asm-branch";
    case NC_ASM_SC_PROBE_POINTER_CHASE: return "asm-pointer-chase";
    case NC_ASM_SC_PROBE_BARRIER: return "asm-barrier";
    case NC_ASM_SC_PROBE_COUNTER: return "asm-counter";
    default: return "asm-unknown";
    }
}

NC_API int nc_asm_sidechannel_arch(void) {
#if defined(__x86_64__) || defined(_M_X64)
    return NC_ASM_SC_ARCH_X64;
#elif defined(__aarch64__) || defined(_M_ARM64)
    return NC_ASM_SC_ARCH_ARM64;
#else
    return NC_ASM_SC_ARCH_NONE;
#endif
}

static uint64_t nc_sc_units_to_ns_(nc_ctx_t *ctx, uint64_t units) {
    if (!units) return 0;
#if defined(__aarch64__) || defined(_M_ARM64)
    uint64_t hz = nc_arm64_cntfrq();
    if (hz) {
#if defined(_MSC_VER) && !defined(__clang__)
        return (uint64_t)((long double)units * 1000000000.0L / (long double)hz);
#else
        return (uint64_t)((((unsigned __int128)units) * 1000000000ull) / hz);
#endif
    }
#endif
    return nc_cycles_to_ns(ctx, units);
}

NC_API uint64_t nc_asm_sidechannel_probe(nc_ctx_t *ctx,
                                         nc_asm_sidechannel_probe_kind_t kind,
                                         void *buffer,
                                         size_t bytes_or_steps,
                                         uint64_t value,
                                         nc_asm_sidechannel_probe_result_t *out) {
    if (out) memset(out, 0, sizeof(*out));
    uint64_t raw = 0;
    uint64_t aux = 0;
    int status = NC_OK;
    int arch = nc_asm_sidechannel_arch();
#if defined(__x86_64__) || defined(_M_X64)
    switch (kind) {
    case NC_ASM_SC_PROBE_COUNTER: {
        uint32_t tsc_aux = 0;
        raw = nc_x64_sc_rdtscp_lfence(&tsc_aux);
        aux = tsc_aux;
        break;
    }
    case NC_ASM_SC_PROBE_LOAD:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_load_cycles(buffer);
        break;
    case NC_ASM_SC_PROBE_LOAD_FENCED:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_load_lfence_cycles(buffer);
        break;
    case NC_ASM_SC_PROBE_FLUSH_RELOAD:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_flush_reload_cycles(buffer);
        break;
    case NC_ASM_SC_PROBE_PREFETCH_RELOAD:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_prefetch_reload_cycles(buffer);
        break;
    case NC_ASM_SC_PROBE_STORE:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_store_cycles(buffer, value);
        break;
    case NC_ASM_SC_PROBE_BRANCH:
        if (!buffer || !bytes_or_steps) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_measure_branch_cycles((const uint8_t*)buffer, bytes_or_steps);
        break;
    case NC_ASM_SC_PROBE_POINTER_CHASE:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_x64_sc_pointer_chase_cycles(buffer, bytes_or_steps ? bytes_or_steps : 1u);
        break;
    case NC_ASM_SC_PROBE_BARRIER:
        raw = nc_x64_sc_barrier_overhead_cycles((uint32_t)(bytes_or_steps ? bytes_or_steps : 1u));
        break;
    default:
        status = NC_ERR_BAD_ARGUMENT;
        break;
    }
#elif defined(__aarch64__) || defined(_M_ARM64)
    switch (kind) {
    case NC_ASM_SC_PROBE_COUNTER:
        raw = nc_arm64_sc_cntvct_isb();
        break;
    case NC_ASM_SC_PROBE_LOAD:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_measure_load_ticks(buffer);
        break;
    case NC_ASM_SC_PROBE_LOAD_FENCED:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_measure_load_isb_ticks(buffer);
        break;
    case NC_ASM_SC_PROBE_PREFETCH_RELOAD:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_measure_prefetch_load_ticks(buffer);
        break;
    case NC_ASM_SC_PROBE_STORE:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_measure_store_ticks(buffer, value);
        break;
    case NC_ASM_SC_PROBE_BRANCH:
        if (!buffer || !bytes_or_steps) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_measure_branch_ticks((const uint8_t*)buffer, bytes_or_steps);
        break;
    case NC_ASM_SC_PROBE_POINTER_CHASE:
        if (!buffer) { status = NC_ERR_BAD_ARGUMENT; break; }
        raw = nc_arm64_sc_pointer_chase_ticks(buffer, bytes_or_steps ? bytes_or_steps : 1u);
        break;
    case NC_ASM_SC_PROBE_BARRIER:
        raw = nc_arm64_sc_barrier_overhead_ticks((uint32_t)(bytes_or_steps ? bytes_or_steps : 1u));
        break;
    case NC_ASM_SC_PROBE_FLUSH_RELOAD:
        status = NC_ERR_UNSUPPORTED;
        break;
    default:
        status = NC_ERR_BAD_ARGUMENT;
        break;
    }
#else
    (void)ctx; (void)kind; (void)buffer; (void)bytes_or_steps; (void)value;
    status = NC_ERR_UNSUPPORTED;
#endif
    if (out) {
        out->status = status;
        out->arch = (uint32_t)arch;
        out->kind = (uint32_t)kind;
        out->raw_units = raw;
        out->ns = status == NC_OK ? nc_sc_units_to_ns_(ctx, raw) : 0;
        out->aux = aux;
        out->checksum = 0;
    }
    return status == NC_OK ? raw : 0;
}

NC_API int nc_asm_sidechannel_cache_audit(nc_ctx_t *ctx,
                                          void *buffer,
                                          nc_asm_sidechannel_probe_result_t *load_out,
                                          nc_asm_sidechannel_probe_result_t *reload_out,
                                          nc_asm_sidechannel_probe_result_t *prefetch_out) {
    if (!buffer) return 0;
    nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_LOAD_FENCED, buffer, 0, 0, load_out);
    nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_FLUSH_RELOAD, buffer, 0, 0, reload_out);
    nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_PREFETCH_RELOAD, buffer, 0, 0, prefetch_out);
    return 1;
}


/* -------------------------------------------------------------------------
 * Low-level SIMD assembly side-channel measurement wrappers.
 * These dispatch to symbols implemented inside each SIMD assembler family file.
 * ------------------------------------------------------------------------- */
#ifndef HWCAP_SVE
#define HWCAP_SVE (1UL << 22)
#endif
#ifndef HWCAP2_SVE2
#define HWCAP2_SVE2 (1UL << 1)
#endif
#ifndef HWCAP2_SME
#define HWCAP2_SME (1UL << 23)
#endif
#ifndef HWCAP2_SME2
#define HWCAP2_SME2 (1UL << 37)
#endif

NC_API const char *nc_asm_simd_family_name(nc_asm_simd_family_t family) {
    switch (family) {
    case NC_ASM_SIMD_MMX: return "mmx";
    case NC_ASM_SIMD_SSE: return "sse";
    case NC_ASM_SIMD_SSE2: return "sse2";
    case NC_ASM_SIMD_SSE3: return "sse3";
    case NC_ASM_SIMD_SSSE3: return "ssse3";
    case NC_ASM_SIMD_SSE41: return "sse4.1";
    case NC_ASM_SIMD_SSE42: return "sse4.2";
    case NC_ASM_SIMD_F16C: return "f16c";
    case NC_ASM_SIMD_FMA: return "fma";
    case NC_ASM_SIMD_AVX: return "avx";
    case NC_ASM_SIMD_AVX2: return "avx2";
    case NC_ASM_SIMD_AVX_VNNI: return "avx-vnni";
    case NC_ASM_SIMD_AVX512: return "avx512";
    case NC_ASM_SIMD_AVX512_VNNI: return "avx512-vnni";
    case NC_ASM_SIMD_ARM64_NEON: return "arm64-neon";
    case NC_ASM_SIMD_ARM64_SVE: return "arm64-sve";
    case NC_ASM_SIMD_ARM64_SVE2: return "arm64-sve2";
    case NC_ASM_SIMD_ARM64_SME: return "arm64-sme";
    case NC_ASM_SIMD_ARM64_SME2: return "arm64-sme2";
    default: return "unknown";
    }
}

NC_API const char *nc_asm_simd_probe_name(nc_asm_simd_probe_kind_t kind) {
    switch (kind) {
    case NC_ASM_SIMD_PROBE_COUNTER: return "simd-counter";
    case NC_ASM_SIMD_PROBE_LOAD: return "simd-load";
    case NC_ASM_SIMD_PROBE_STORE: return "simd-store";
    case NC_ASM_SIMD_PROBE_VECTOR_LOAD: return "simd-vector-load";
    case NC_ASM_SIMD_PROBE_VECTOR_XOR: return "simd-vector-xor";
    case NC_ASM_SIMD_PROBE_BARRIER: return "simd-barrier";
    default: return "simd-unknown";
    }
}

NC_API int nc_asm_simd_family_available(nc_asm_simd_family_t family) {
#if defined(__x86_64__) || defined(_M_X64)
    switch (family) {
    case NC_ASM_SIMD_MMX: return nc_cpu_has_mmx();
    case NC_ASM_SIMD_SSE: return nc_cpu_has_sse();
    case NC_ASM_SIMD_SSE2: return nc_cpu_has_sse2();
    case NC_ASM_SIMD_SSE3: return nc_cpu_has_sse3();
    case NC_ASM_SIMD_SSSE3: return nc_cpu_has_ssse3();
    case NC_ASM_SIMD_SSE41: return nc_cpu_has_sse41();
    case NC_ASM_SIMD_SSE42: return nc_cpu_has_sse42();
    case NC_ASM_SIMD_F16C: return nc_cpu_has_f16c();
    case NC_ASM_SIMD_FMA: return nc_cpu_has_fma();
    case NC_ASM_SIMD_AVX: return nc_is_avx_available();
    case NC_ASM_SIMD_AVX2: return nc_cpu_has_avx2();
    case NC_ASM_SIMD_AVX_VNNI: return nc_cpu_has_avx_vnni();
    case NC_ASM_SIMD_AVX512: return nc_cpu_has_avx512f();
    case NC_ASM_SIMD_AVX512_VNNI: return nc_cpu_has_avx512vnni();
    default: return 0;
    }
#elif defined(__aarch64__) || defined(_M_ARM64)
    switch (family) {
    case NC_ASM_SIMD_ARM64_NEON: return 1;
#if defined(__linux__)
    case NC_ASM_SIMD_ARM64_SVE: return (getauxval(AT_HWCAP) & HWCAP_SVE) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SVE2: return (getauxval(AT_HWCAP2) & HWCAP2_SVE2) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SME: return (getauxval(AT_HWCAP2) & HWCAP2_SME) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SME2: return (getauxval(AT_HWCAP2) & HWCAP2_SME2) ? 1 : 0;
#elif defined(_WIN32)
#ifndef PF_ARM_SVE_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE_INSTRUCTIONS_AVAILABLE 46
#endif
#ifndef PF_ARM_SVE2_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE2_INSTRUCTIONS_AVAILABLE 47
#endif
#ifndef PF_ARM_SME_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SME_INSTRUCTIONS_AVAILABLE 70
#endif
#ifndef PF_ARM_SME2_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SME2_INSTRUCTIONS_AVAILABLE 71
#endif
    case NC_ASM_SIMD_ARM64_SVE: return IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SVE2: return IsProcessorFeaturePresent(PF_ARM_SVE2_INSTRUCTIONS_AVAILABLE) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SME: return IsProcessorFeaturePresent(PF_ARM_SME_INSTRUCTIONS_AVAILABLE) ? 1 : 0;
    case NC_ASM_SIMD_ARM64_SME2: return IsProcessorFeaturePresent(PF_ARM_SME2_INSTRUCTIONS_AVAILABLE) ? 1 : 0;
#else
    case NC_ASM_SIMD_ARM64_SVE:
    case NC_ASM_SIMD_ARM64_SVE2:
    case NC_ASM_SIMD_ARM64_SME:
    case NC_ASM_SIMD_ARM64_SME2: return 0;
#endif
    default: return 0;
    }
#else
    (void)family;
    return 0;
#endif
}

typedef uint64_t (*nc_simd_counter_fn)(void);
typedef uint64_t (*nc_simd_load_fn)(const void *);
typedef uint64_t (*nc_simd_store_fn)(void *, uint64_t);
typedef uint64_t (*nc_simd_xor_fn)(const void *, const void *, void *, size_t);
typedef uint64_t (*nc_simd_barrier_fn)(uint32_t);

typedef struct nc_simd_dispatch_entry {
    nc_simd_counter_fn counter;
    nc_simd_load_fn load;
    nc_simd_store_fn store;
    nc_simd_load_fn vector_load;
    nc_simd_xor_fn vector_xor;
    nc_simd_barrier_fn barrier;
} nc_simd_dispatch_entry_t;

#define NC_X64_SIMD_ENTRY(pfx) { nc_##pfx##_sc_counter, nc_##pfx##_sc_load_cycles, nc_##pfx##_sc_store_cycles, nc_##pfx##_sc_vector_load_cycles, nc_##pfx##_sc_vector_xor_cycles, nc_##pfx##_sc_barrier_cycles }
#define NC_ARM64_SIMD_ENTRY(pfx) { nc_arm64_##pfx##_sc_counter, nc_arm64_##pfx##_sc_load_ticks, nc_arm64_##pfx##_sc_store_ticks, nc_arm64_##pfx##_sc_vector_load_ticks, nc_arm64_##pfx##_sc_vector_xor_ticks, nc_arm64_##pfx##_sc_barrier_ticks }

static int nc_asm_simd_dispatch_(nc_asm_simd_family_t family, nc_simd_dispatch_entry_t *entry) {
    if (!entry) return 0;
    memset(entry, 0, sizeof(*entry));
#if defined(__x86_64__) || defined(_M_X64)
    switch (family) {
    case NC_ASM_SIMD_MMX: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(mmx); return 1;
    case NC_ASM_SIMD_SSE: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(sse); return 1;
    case NC_ASM_SIMD_SSE2: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(sse2); return 1;
    case NC_ASM_SIMD_SSE3: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(sse3); return 1;
    case NC_ASM_SIMD_SSSE3: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(ssse3); return 1;
    case NC_ASM_SIMD_SSE41: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(sse41); return 1;
    case NC_ASM_SIMD_SSE42: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(sse42); return 1;
    case NC_ASM_SIMD_F16C: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(f16c); return 1;
    case NC_ASM_SIMD_FMA: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(fma); return 1;
    case NC_ASM_SIMD_AVX: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(avx); return 1;
    case NC_ASM_SIMD_AVX2: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(avx2); return 1;
    case NC_ASM_SIMD_AVX_VNNI: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(avx_vnni); return 1;
    case NC_ASM_SIMD_AVX512: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(avx512); return 1;
    case NC_ASM_SIMD_AVX512_VNNI: *entry = (nc_simd_dispatch_entry_t)NC_X64_SIMD_ENTRY(avx512_vnni); return 1;
    default: return 0;
    }
#elif defined(__aarch64__) || defined(_M_ARM64)
    switch (family) {
    case NC_ASM_SIMD_ARM64_NEON: *entry = (nc_simd_dispatch_entry_t)NC_ARM64_SIMD_ENTRY(neon); return 1;
    case NC_ASM_SIMD_ARM64_SVE: *entry = (nc_simd_dispatch_entry_t)NC_ARM64_SIMD_ENTRY(sve); return 1;
    case NC_ASM_SIMD_ARM64_SVE2: *entry = (nc_simd_dispatch_entry_t)NC_ARM64_SIMD_ENTRY(sve2); return 1;
    case NC_ASM_SIMD_ARM64_SME: *entry = (nc_simd_dispatch_entry_t)NC_ARM64_SIMD_ENTRY(sme); return 1;
    case NC_ASM_SIMD_ARM64_SME2: *entry = (nc_simd_dispatch_entry_t)NC_ARM64_SIMD_ENTRY(sme2); return 1;
    default: return 0;
    }
#else
    (void)family;
    return 0;
#endif
}

static uint64_t nc_asm_simd_checksum_(const void *p, size_t bytes) {
    const uint8_t *b = (const uint8_t*)p;
    uint64_t x = 0xcbf29ce484222325ull;
    if (!b) return 0;
    for (size_t i = 0; i < bytes; ++i) x = (x ^ b[i]) * 0x100000001b3ull;
    return x;
}

NC_API uint64_t nc_asm_simd_probe(nc_ctx_t *ctx,
                                  nc_asm_simd_family_t family,
                                  nc_asm_simd_probe_kind_t kind,
                                  void *a,
                                  void *b,
                                  void *out_buf,
                                  size_t bytes_or_iterations,
                                  uint64_t value,
                                  nc_asm_simd_probe_result_t *out) {
    if (out) memset(out, 0, sizeof(*out));
    int status = NC_OK;
    uint64_t raw = 0;
    nc_simd_dispatch_entry_t e;
    if (!nc_asm_simd_dispatch_(family, &e)) status = NC_ERR_UNSUPPORTED;
    else if (!nc_asm_simd_family_available(family)) status = NC_ERR_UNSUPPORTED;
    else {
        switch (kind) {
        case NC_ASM_SIMD_PROBE_COUNTER:
            raw = e.counter ? e.counter() : 0;
            break;
        case NC_ASM_SIMD_PROBE_LOAD:
            if (!a) status = NC_ERR_BAD_ARGUMENT;
            else raw = e.load ? e.load(a) : 0;
            break;
        case NC_ASM_SIMD_PROBE_STORE:
            if (!a) status = NC_ERR_BAD_ARGUMENT;
            else raw = e.store ? e.store(a, value) : 0;
            break;
        case NC_ASM_SIMD_PROBE_VECTOR_LOAD:
            if (!a) status = NC_ERR_BAD_ARGUMENT;
            else raw = e.vector_load ? e.vector_load(a) : 0;
            break;
        case NC_ASM_SIMD_PROBE_VECTOR_XOR:
            if (!a || !b || !out_buf) status = NC_ERR_BAD_ARGUMENT;
            else raw = e.vector_xor ? e.vector_xor(a, b, out_buf, bytes_or_iterations) : 0;
            break;
        case NC_ASM_SIMD_PROBE_BARRIER:
            raw = e.barrier ? e.barrier((uint32_t)(bytes_or_iterations ? bytes_or_iterations : 1u)) : 0;
            break;
        default:
            status = NC_ERR_BAD_ARGUMENT;
            break;
        }
    }
    if (out) {
        out->status = status;
        out->family = (uint32_t)family;
        out->kind = (uint32_t)kind;
        out->raw_units = raw;
        out->ns = status == NC_OK ? nc_sc_units_to_ns_(ctx, raw) : 0;
        out->bytes = bytes_or_iterations;
        out->checksum = out_buf ? nc_asm_simd_checksum_(out_buf, bytes_or_iterations > 512 ? 512 : bytes_or_iterations) : 0;
    }
    return status == NC_OK ? raw : 0;
}


/* -------------------------------------------------------------------------
 * Nanosecond clock + auto-route + wrapper-overhead API
 * ------------------------------------------------------------------------- */
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if defined(_WIN32)
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <sched.h>
#  include <pthread.h>
#endif

#ifndef NC_ARCH_UNKNOWN_
#define NC_ARCH_UNKNOWN_ 0u
#define NC_ARCH_X64_     1u
#define NC_ARCH_ARM64_   2u
#endif

#if !defined(__x86_64__) && !defined(_M_X64)
NC_API uint64_t nc_x64_sc_rdtsc_mfence(void) { return 0; }
#endif
#if !defined(__aarch64__) && !defined(_M_ARM64)
NC_API uint64_t nc_arm64_pmccntr_el0(void) { return 0; }
#endif

NC_API uint64_t nc_unix_time_ns(void) {
#if defined(_WIN32)
    FILETIME ft;
#  if defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x0602
    GetSystemTimePreciseAsFileTime(&ft);
#  else
    GetSystemTimeAsFileTime(&ft);
#  endif
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    const uint64_t unix_epoch_100ns = 116444736000000000ull;
    if (u.QuadPart < unix_epoch_100ns) return 0;
    return (u.QuadPart - unix_epoch_100ns) * 100ull;
#elif defined(CLOCK_REALTIME)
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
    return nc_wall_time_ns();
#else
    time_t t = time(NULL);
    return t > 0 ? (uint64_t)t * 1000000000ull : nc_wall_time_ns();
#endif
}

NC_API const char *nc_clock_route_name(nc_clock_route_t route) {
    switch (route) {
    case NC_CLOCK_ROUTE_AUTO: return "auto";
    case NC_CLOCK_ROUTE_WALL_REALTIME_NS: return "wall-realtime-ns";
    case NC_CLOCK_ROUTE_MONOTONIC_NS: return "monotonic-ns";
    case NC_CLOCK_ROUTE_X64_RDTSC_RAW: return "x64-rdtsc-raw";
    case NC_CLOCK_ROUTE_X64_RDTSC_LFENCE: return "x64-lfence-rdtsc";
    case NC_CLOCK_ROUTE_X64_RDTSC_MFENCE: return "x64-mfence-lfence-rdtsc";
    case NC_CLOCK_ROUTE_X64_RDTSCP_LFENCE: return "x64-rdtscp-lfence";
    case NC_CLOCK_ROUTE_ARM64_CNTFRQ_EL0: return "arm64-cntfrq-el0";
    case NC_CLOCK_ROUTE_ARM64_CNTVCT_EL0: return "arm64-cntvct-el0";
    case NC_CLOCK_ROUTE_ARM64_CNTVCT_ISB: return "arm64-isb-cntvct-el0";
    case NC_CLOCK_ROUTE_ARM64_PMCCNTR_EL0: return "arm64-isb-pmccntr-el0";
    case NC_CLOCK_ROUTE_BEST_SIMD_COUNTER: return "best-simd-counter";
    default: return "unknown";
    }
}

NC_API const char *nc_wrapper_kind_name(nc_wrapper_kind_t kind) {
    switch (kind) {
    case NC_WRAPPER_C_API: return "c-api";
    case NC_WRAPPER_DLL_BOUNDARY: return "dll-boundary";
    case NC_WRAPPER_PYTHON_CTYPES: return "python-ctypes";
    case NC_WRAPPER_PYTHON_CFFI: return "python-cffi";
    case NC_WRAPPER_NODE_FFI: return "node-ffi";
    case NC_WRAPPER_LUA_FFI: return "lua-ffi";
    case NC_WRAPPER_JAVA_JNA: return "java-jna";
    case NC_WRAPPER_CSHARP_PINVOKE: return "csharp-pinvoke";
    case NC_WRAPPER_GO_CGO: return "go-cgo";
    case NC_WRAPPER_RUST_FFI: return "rust-ffi";
    case NC_WRAPPER_ZIG_FFI: return "zig-ffi";
    default: return "unknown";
    }
}

NC_API nc_asm_simd_family_t nc_select_best_simd_family(void) {
#if defined(__x86_64__) || defined(_M_X64)
    static const nc_asm_simd_family_t order[] = {
        NC_ASM_SIMD_AVX512_VNNI, NC_ASM_SIMD_AVX512, NC_ASM_SIMD_AVX_VNNI,
        NC_ASM_SIMD_AVX2, NC_ASM_SIMD_AVX, NC_ASM_SIMD_FMA, NC_ASM_SIMD_F16C,
        NC_ASM_SIMD_SSE42, NC_ASM_SIMD_SSE41, NC_ASM_SIMD_SSSE3, NC_ASM_SIMD_SSE3,
        NC_ASM_SIMD_SSE2, NC_ASM_SIMD_SSE, NC_ASM_SIMD_MMX
    };
#elif defined(__aarch64__) || defined(_M_ARM64)
    static const nc_asm_simd_family_t order[] = {
        NC_ASM_SIMD_ARM64_SME2, NC_ASM_SIMD_ARM64_SME, NC_ASM_SIMD_ARM64_SVE2, NC_ASM_SIMD_ARM64_SVE, NC_ASM_SIMD_ARM64_NEON
    };
#else
    static const nc_asm_simd_family_t order[] = { NC_ASM_SIMD_MMX };
#endif
    for (size_t i = 0; i < NC_ARRAY_COUNT(order); ++i) {
        if (nc_asm_simd_family_available(order[i])) return order[i];
    }
#if defined(__aarch64__) || defined(_M_ARM64)
    return NC_ASM_SIMD_ARM64_NEON;
#else
    return NC_ASM_SIMD_SSE2;
#endif
}

NC_API int nc_arm64_pmccntr_el0_available(void) {
#if defined(__aarch64__) || defined(_M_ARM64)
    const char *e = getenv("NANOCHRONO_USE_PMCCNTR_EL0");
    return e && (e[0] == '1' || e[0] == 'y' || e[0] == 'Y' || e[0] == 't' || e[0] == 'T');
#else
    return 0;
#endif
}

NC_API nc_clock_route_t nc_clock_select_best_route(void) {
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
    nc_asm_simd_family_t f = nc_select_best_simd_family();
    if (nc_asm_simd_family_available(f)) return NC_CLOCK_ROUTE_BEST_SIMD_COUNTER;
#endif
#if defined(__x86_64__) || defined(_M_X64)
    return NC_CLOCK_ROUTE_X64_RDTSCP_LFENCE;
#elif defined(__aarch64__) || defined(_M_ARM64)
    if (nc_arm64_pmccntr_el0_available()) return NC_CLOCK_ROUTE_ARM64_PMCCNTR_EL0;
    return NC_CLOCK_ROUTE_ARM64_CNTVCT_ISB;
#else
    return NC_CLOCK_ROUTE_MONOTONIC_NS;
#endif
}

NC_API uint64_t nc_nanoclock_now_ns(nc_ctx_t *ctx, nc_clock_route_t route, uint64_t *raw_units_out) {
    if (route == NC_CLOCK_ROUTE_AUTO) route = nc_clock_select_best_route();
    uint64_t raw = 0, ns = 0;
    switch (route) {
    case NC_CLOCK_ROUTE_WALL_REALTIME_NS:
        ns = nc_unix_time_ns(); raw = ns; break;
    case NC_CLOCK_ROUTE_MONOTONIC_NS:
        ns = nc_monotonic_time_ns(); raw = ns; break;
#if defined(__x86_64__) || defined(_M_X64)
    case NC_CLOCK_ROUTE_X64_RDTSC_RAW:
        raw = nc_tsc_raw(); ns = nc_cycles_to_ns(ctx, raw); break;
    case NC_CLOCK_ROUTE_X64_RDTSC_LFENCE:
        raw = nc_x64_sc_rdtsc_lfence(); ns = nc_cycles_to_ns(ctx, raw); break;
    case NC_CLOCK_ROUTE_X64_RDTSC_MFENCE:
        raw = nc_x64_sc_rdtsc_mfence(); ns = nc_cycles_to_ns(ctx, raw); break;
    case NC_CLOCK_ROUTE_X64_RDTSCP_LFENCE:
        raw = nc_x64_sc_rdtscp_lfence(NULL); ns = nc_cycles_to_ns(ctx, raw); break;
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
    case NC_CLOCK_ROUTE_ARM64_CNTFRQ_EL0:
        raw = nc_arm64_cntfrq(); ns = raw; break;
    case NC_CLOCK_ROUTE_ARM64_CNTVCT_EL0:
        raw = nc_arm64_cntvct(); ns = nc_sc_units_to_ns_(ctx, raw); break;
    case NC_CLOCK_ROUTE_ARM64_CNTVCT_ISB:
        raw = nc_arm64_sc_cntvct_isb(); ns = nc_sc_units_to_ns_(ctx, raw); break;
    case NC_CLOCK_ROUTE_ARM64_PMCCNTR_EL0:
        if (nc_arm64_pmccntr_el0_available()) { raw = nc_arm64_pmccntr_el0(); ns = nc_sc_units_to_ns_(ctx, raw); }
        else { raw = 0; ns = 0; }
        break;
#endif
    case NC_CLOCK_ROUTE_BEST_SIMD_COUNTER: {
        nc_asm_simd_family_t fam = nc_select_best_simd_family();
        raw = nc_asm_simd_probe(ctx, fam, NC_ASM_SIMD_PROBE_COUNTER, NULL, NULL, NULL, 0, 0, NULL);
        ns = nc_sc_units_to_ns_(ctx, raw);
        break;
    }
    default:
        ns = nc_monotonic_time_ns(); raw = ns; break;
    }
    if (raw_units_out) *raw_units_out = raw;
    return ns;
}

NC_API uint64_t nc_measure_wrapper_overhead_cycles(nc_ctx_t *ctx, nc_wrapper_kind_t kind, uint32_t iterations) {
    if (!iterations) iterations = 10000;
    uint64_t native = nc_measure_ffi_overhead_cycles(ctx, iterations);
    switch (kind) {
    case NC_WRAPPER_C_API: return native;
    case NC_WRAPPER_DLL_BOUNDARY: return nc_measure_dll_boundary_cycles(ctx, iterations);
    default: return native;
    }
}

NC_API int nc_nanoclock_snapshot(nc_ctx_t *ctx, nc_nanoclock_snapshot_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
#if defined(__x86_64__) || defined(_M_X64)
    out->arch = NC_ARCH_X64_;
#elif defined(__aarch64__) || defined(_M_ARM64)
    out->arch = NC_ARCH_ARM64_;
#else
    out->arch = NC_ARCH_UNKNOWN_;
#endif
    out->backend = (uint32_t)nc_backend(ctx);
    out->best_simd_family = (uint32_t)nc_select_best_simd_family();
    out->route = (uint32_t)nc_clock_select_best_route();
    out->unix_time_ns = nc_unix_time_ns();
    out->monotonic_ns = nc_monotonic_time_ns();
    out->process_time_ns = nc_process_time_ns();
    out->thread_time_ns = nc_thread_time_ns();
    out->native_overhead_cycles = nc_measure_overhead_cycles(ctx);
    out->ffi_overhead_cycles = nc_measure_ffi_overhead_cycles(ctx, 1000);
    out->dll_boundary_cycles = nc_measure_dll_boundary_cycles(ctx, 1000);
    out->wrapper_hint_cycles = nc_measure_wrapper_overhead_cycles(ctx, NC_WRAPPER_PYTHON_CTYPES, 1000);
#if defined(__x86_64__) || defined(_M_X64)
    out->rdtsc_raw = nc_tsc_raw();
    out->rdtsc_lfence = nc_x64_sc_rdtsc_lfence();
    out->rdtsc_mfence = nc_x64_sc_rdtsc_mfence();
    out->rdtscp_lfence = nc_x64_sc_rdtscp_lfence(&out->rdtscp_aux);
    out->selected_overhead_units = nc_x64_sc_barrier_overhead_cycles(64);
#elif defined(__aarch64__) || defined(_M_ARM64)
    out->arm64_cntfrq_el0 = nc_arm64_cntfrq();
    out->arm64_cntvct_el0 = nc_arm64_cntvct();
    out->arm64_cntvct_isb = nc_arm64_sc_cntvct_isb();
    out->arm64_cntvct_ns = nc_sc_units_to_ns_(ctx, out->arm64_cntvct_isb);
    out->arm64_pmccntr_available = (uint32_t)nc_arm64_pmccntr_el0_available();
    if (out->arm64_pmccntr_available) {
        out->arm64_pmccntr_el0 = nc_arm64_pmccntr_el0();
        out->arm64_pmccntr_ns = nc_sc_units_to_ns_(ctx, out->arm64_pmccntr_el0);
        out->arm64_pmccntr_status = NC_OK;
    } else {
        out->arm64_pmccntr_status = NC_ERR_UNSUPPORTED;
    }
    out->selected_overhead_units = nc_arm64_sc_barrier_overhead_ticks(64);
#endif
    out->best_simd_counter = nc_asm_simd_probe(ctx, (nc_asm_simd_family_t)out->best_simd_family,
                                               NC_ASM_SIMD_PROBE_COUNTER, NULL, NULL, NULL, 0, 0, NULL);
    out->best_simd_counter_ns = nc_sc_units_to_ns_(ctx, out->best_simd_counter);
    out->selected_ns = nc_nanoclock_now_ns(ctx, (nc_clock_route_t)out->route, &out->selected_raw_units);
    return 1;
}

NC_API void nc_format_unix_time_ns(uint64_t unix_ns, char *buf, size_t cap) {
    if (!buf || !cap) return;
    time_t sec = (time_t)(unix_ns / 1000000000ull);
    unsigned long nsec = (unsigned long)(unix_ns % 1000000000ull);
    struct tm tmv;
#if defined(_WIN32)
    gmtime_s(&tmv, &sec);
#else
    struct tm *ptm = gmtime(&sec);
    if (ptm) tmv = *ptm; else memset(&tmv, 0, sizeof(tmv));
#endif
    snprintf(buf, cap, "%04d-%02d-%02d %02d:%02d:%02d.%09luZ",
             tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
             tmv.tm_hour, tmv.tm_min, tmv.tm_sec, nsec);
}


/* -------------------------------------------------------------------------
 * Precision Clock App API: raw local time + optional NTP discipline.
 * Critical path rule: raw clock reads never touch network and do not require
 * a kernel call when an architectural counter route is selected. NTP is an
 * explicit, separately measured synchronisation step.
 * ------------------------------------------------------------------------- */
NC_API const char *nc_clock_view_name(nc_clock_view_t view) {
    switch (view) {
    case NC_CLOCK_VIEW_STOPWATCH: return "stopwatch";
    case NC_CLOCK_VIEW_CLOCK: return "clock";
    case NC_CLOCK_VIEW_TIMER: return "timer";
    default: return "unknown";
    }
}

NC_API const char *nc_clock_detail_mode_name(nc_clock_detail_mode_t mode) {
    switch (mode) {
    case NC_CLOCK_DETAIL_SIMPLE: return "simple";
    case NC_CLOCK_DETAIL_NANO: return "nano";
    default: return "unknown";
    }
}

NC_API const char *nc_time_zone_mode_name(nc_time_zone_mode_t mode) {
    switch (mode) {
    case NC_TIME_ZONE_LOCAL: return "local";
    case NC_TIME_ZONE_UTC: return "utc";
    case NC_TIME_ZONE_CUSTOM_OFFSET: return "utc-offset";
    default: return "unknown";
    }
}

static uint64_t nc_clock_read_route_raw_(nc_ctx_t *ctx, nc_clock_route_t route) {
    uint64_t raw = 0;
    (void)nc_nanoclock_now_ns(ctx, route, &raw);
    return raw;
}

NC_API int nc_format_unix_time_ns_ex(uint64_t unix_ns, nc_time_zone_mode_t mode,
                                     int32_t utc_offset_minutes, char *buf, size_t cap) {
    if (!buf || cap == 0) return 0;
    int32_t offset_min = 0;
    const char *suffix = "Z";
    if (mode == NC_TIME_ZONE_LOCAL) {
#if defined(_WIN32)
        TIME_ZONE_INFORMATION tzi;
        DWORD r = GetTimeZoneInformation(&tzi);
        LONG bias = tzi.Bias;
        if (r == TIME_ZONE_ID_DAYLIGHT) bias += tzi.DaylightBias;
        else if (r == TIME_ZONE_ID_STANDARD) bias += tzi.StandardBias;
        offset_min = (int32_t)(-bias);
#else
        time_t now = (time_t)(unix_ns / 1000000000ull);
        struct tm gm, loc;
        struct tm *pg = gmtime(&now);
        struct tm *pl = localtime(&now);
        if (pg && pl) {
            gm = *pg; loc = *pl;
            time_t gmt_as_local = mktime(&gm);
            time_t loc_as_local = mktime(&loc);
            offset_min = (int32_t)difftime(loc_as_local, gmt_as_local) / 60;
        }
#endif
        suffix = "LOCAL";
    } else if (mode == NC_TIME_ZONE_CUSTOM_OFFSET) {
        offset_min = utc_offset_minutes;
        suffix = "UTC-OFFSET";
    }
    int64_t adj = (int64_t)unix_ns + (int64_t)offset_min * 60ll * 1000000000ll;
    if (adj < 0) adj = 0;
    time_t sec = (time_t)((uint64_t)adj / 1000000000ull);
    unsigned long nsec = (unsigned long)((uint64_t)adj % 1000000000ull);
    struct tm tmv;
#if defined(_WIN32)
    gmtime_s(&tmv, &sec);
#else
    struct tm *ptm = gmtime(&sec);
    if (ptm) tmv = *ptm; else memset(&tmv, 0, sizeof(tmv));
#endif
    char off[16] = "Z";
    if (mode != NC_TIME_ZONE_UTC) {
        int m = offset_min < 0 ? -offset_min : offset_min;
        snprintf(off, sizeof(off), "%c%02d:%02d", offset_min < 0 ? '-' : '+', m / 60, m % 60);
    }
    snprintf(buf, cap, "%04d-%02d-%02d %02d:%02d:%02d.%09lu %s %s",
             tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday,
             tmv.tm_hour, tmv.tm_min, tmv.tm_sec, nsec, off, suffix);
    return 1;
}

NC_API int nc_precision_clock_default_config(nc_precision_clock_config_t *cfg) {
    if (!cfg) return 0;
    memset(cfg, 0, sizeof(*cfg));
    cfg->view = NC_CLOCK_VIEW_CLOCK;
    cfg->detail_mode = NC_CLOCK_DETAIL_SIMPLE;
    cfg->zone_mode = NC_TIME_ZONE_LOCAL;
    cfg->route = NC_CLOCK_ROUTE_AUTO;
    cfg->cpu_index = 0;
    cfg->pin_cpu = 0;
    cfg->ntp_timeout_ms = 750;
    snprintf(cfg->ntp_server, sizeof(cfg->ntp_server), "%s", "pool.ntp.org");
    return 1;
}

NC_API uint32_t nc_clock_current_cpu(void) {
#if defined(_WIN32)
    return (uint32_t)GetCurrentProcessorNumber();
#elif defined(__linux__)
    int c = sched_getcpu();
    return c < 0 ? 0xffffffffu : (uint32_t)c;
#else
    return 0xffffffffu;
#endif
}

NC_API int nc_clock_pin_thread_to_cpu(uint32_t cpu_index) {
#if defined(_WIN32)
    DWORD_PTR mask = ((DWORD_PTR)1) << (cpu_index % (sizeof(DWORD_PTR) * 8));
    return SetThreadAffinityMask(GetCurrentThread(), mask) ? 1 : 0;
#elif defined(__ANDROID__)
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET((int)cpu_index, &set);
#  if defined(__NR_gettid) && defined(__NR_sched_setaffinity)
    long tid = syscall(__NR_gettid);
    if (tid < 0) return 0;
    return syscall(__NR_sched_setaffinity, tid, sizeof(set), &set) == 0 ? 1 : 0;
#  else
    return 0;
#  endif
#elif defined(__linux__)
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET((int)cpu_index, &set);
    return pthread_setaffinity_np(pthread_self(), sizeof(set), &set) == 0 ? 1 : 0;
#else
    (void)cpu_index;
    return 0;
#endif
}

NC_API uint64_t nc_measure_kernel_timecall_overhead_cycles(nc_ctx_t *ctx, uint32_t iterations) {
    if (!iterations) iterations = 10000;
    uint64_t best = UINT64_MAX;
    for (uint32_t i = 0; i < iterations; ++i) {
        uint64_t a = nc_clock_read_route_raw_(ctx, nc_clock_select_best_route());
        volatile uint64_t t = nc_unix_time_ns();
        uint64_t b = nc_clock_read_route_raw_(ctx, nc_clock_select_best_route());
        (void)t;
        uint64_t d = (b >= a) ? (b - a) : 0;
        if (d && d < best) best = d;
    }
    return best == UINT64_MAX ? 0 : best;
}

NC_API uint64_t nc_measure_api_call_overhead_cycles(nc_ctx_t *ctx, uint32_t iterations) {
    if (!iterations) iterations = 10000;
    uint64_t best = UINT64_MAX;
    for (uint32_t i = 0; i < iterations; ++i) {
        uint64_t a = nc_clock_read_route_raw_(ctx, nc_clock_select_best_route());
        volatile uint64_t x = nc_nanoclock_now_ns(ctx, nc_clock_select_best_route(), NULL);
        uint64_t b = nc_clock_read_route_raw_(ctx, nc_clock_select_best_route());
        (void)x;
        uint64_t d = (b >= a) ? (b - a) : 0;
        if (d && d < best) best = d;
    }
    return best == UINT64_MAX ? 0 : best;
}

NC_API int nc_calibrate_clock_route(nc_ctx_t *ctx, nc_clock_route_t route,
                                    uint32_t samples, uint32_t pin_cpu,
                                    uint32_t cpu_index,
                                    nc_clock_calibration_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    out->route = (uint32_t)(route == NC_CLOCK_ROUTE_AUTO ? nc_clock_select_best_route() : route);
    out->samples = samples ? samples : 100000;
    if (pin_cpu) out->pinned = (uint32_t)nc_clock_pin_thread_to_cpu(cpu_index);
    out->cpu_before = nc_clock_current_cpu();
    uint64_t t0 = nc_monotonic_time_ns();
    uint64_t r0 = nc_clock_read_route_raw_(ctx, (nc_clock_route_t)out->route);
    for (uint64_t i = 0; i < out->samples; ++i) {
        volatile uint64_t v = nc_clock_read_route_raw_(ctx, (nc_clock_route_t)out->route);
        (void)v;
    }
    uint64_t r1 = nc_clock_read_route_raw_(ctx, (nc_clock_route_t)out->route);
    uint64_t t1 = nc_monotonic_time_ns();
    out->cpu_after = nc_clock_current_cpu();
    out->migrated = (out->cpu_before != 0xffffffffu && out->cpu_after != 0xffffffffu && out->cpu_before != out->cpu_after) ? 1u : 0u;
    out->elapsed_raw_units = (r1 >= r0) ? (r1 - r0) : 0;
    out->elapsed_ns = (t1 >= t0) ? (t1 - t0) : 0;
    if (out->elapsed_ns && out->elapsed_raw_units) {
        out->units_per_second = (double)out->elapsed_raw_units * 1.0e9 / (double)out->elapsed_ns;
        out->ns_per_unit = (double)out->elapsed_ns / (double)out->elapsed_raw_units;
    }
    out->read_overhead_units = nc_measure_api_call_overhead_cycles(ctx, 256);
    out->kernel_timecall_overhead_cycles = nc_measure_kernel_timecall_overhead_cycles(ctx, 256);
    out->api_call_overhead_cycles = nc_measure_api_call_overhead_cycles(ctx, 256);
    (void)ctx;
    out->ppm_error_vs_context = 0.0;
    return 1;
}

static uint64_t nc_ntp_ts_to_unix_ns_(const unsigned char *p) {
    uint32_t sec = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
    uint32_t frac = ((uint32_t)p[4] << 24) | ((uint32_t)p[5] << 16) | ((uint32_t)p[6] << 8) | (uint32_t)p[7];
    const uint64_t ntp_unix_delta = 2208988800ull;
    if (sec < ntp_unix_delta) return 0;
    uint64_t unix_sec = (uint64_t)sec - ntp_unix_delta;
    uint64_t ns_frac = ((uint64_t)frac * 1000000000ull) >> 32;
    return unix_sec * 1000000000ull + ns_frac;
}

NC_API int nc_ntp_query(nc_ctx_t *ctx, const char *server, uint32_t timeout_ms,
                        nc_clock_route_t route, nc_ntp_sample_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_ERR_UNSUPPORTED;
    if (!server || !server[0]) server = "pool.ntp.org";
    snprintf(out->server, sizeof(out->server), "%s", server);
    if (!timeout_ms) timeout_ms = 750;
    if (route == NC_CLOCK_ROUTE_AUTO) route = nc_clock_select_best_route();
    out->route = (uint32_t)route;
    out->cpu_before = nc_clock_current_cpu();
    out->kernel_timecall_overhead_cycles = nc_measure_kernel_timecall_overhead_cycles(ctx, 128);
    out->api_call_overhead_cycles = nc_measure_api_call_overhead_cycles(ctx, 128);

#if defined(_WIN32)
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 0;
#endif
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_UNSPEC;
    uint64_t setup0 = nc_clock_read_route_raw_(ctx, route);
    int gai = getaddrinfo(server, "123", &hints, &res);
    uint64_t setup1 = nc_clock_read_route_raw_(ctx, route);
    out->socket_setup_overhead_units = setup1 >= setup0 ? setup1 - setup0 : 0;
    if (gai != 0 || !res) {
#if defined(_WIN32)
        WSACleanup();
#endif
        return 0;
    }
    int ok = 0;
    for (struct addrinfo *rp = res; rp && !ok; rp = rp->ai_next) {
#if defined(_WIN32)
        SOCKET fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == INVALID_SOCKET) continue;
        DWORD tv = timeout_ms;
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#else
        int fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0) continue;
        struct timeval tv;
        tv.tv_sec = (int)(timeout_ms / 1000u);
        tv.tv_usec = (int)((timeout_ms % 1000u) * 1000u);
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif
        unsigned char pkt[48];
        memset(pkt, 0, sizeof(pkt));
        pkt[0] = 0x23; /* LI=0, VN=4, mode=3 client */
        out->raw_before_units = nc_clock_read_route_raw_(ctx, route);
        out->local_send_unix_ns = nc_unix_time_ns();
        uint64_t sr0 = nc_clock_read_route_raw_(ctx, route);
        int sent = (int)sendto(fd, (const char *)pkt, sizeof(pkt), 0, rp->ai_addr, (int)rp->ai_addrlen);
        int recvd = -1;
        if (sent == (int)sizeof(pkt)) {
            recvd = (int)recvfrom(fd, (char *)pkt, sizeof(pkt), 0, NULL, NULL);
        }
        uint64_t sr1 = nc_clock_read_route_raw_(ctx, route);
        out->local_recv_unix_ns = nc_unix_time_ns();
        out->raw_after_units = nc_clock_read_route_raw_(ctx, route);
        out->send_recv_overhead_units = sr1 >= sr0 ? sr1 - sr0 : 0;
#if defined(_WIN32)
        closesocket(fd);
#else
        close(fd);
#endif
        if (recvd >= 48) {
            out->leap_indicator = (pkt[0] >> 6) & 3u;
            out->version = (pkt[0] >> 3) & 7u;
            out->mode = pkt[0] & 7u;
            out->stratum = pkt[1];
            out->precision_exp = (int8_t)pkt[3];
            out->ntp_transmit_unix_ns = nc_ntp_ts_to_unix_ns_(pkt + 40);
            uint64_t t1 = out->local_send_unix_ns;
            uint64_t t4 = out->local_recv_unix_ns;
            uint64_t t3 = out->ntp_transmit_unix_ns;
            if (t3 && t4 >= t1) {
                out->delay_ns = t4 - t1;
                int64_t midpoint = (int64_t)(t1 + (t4 - t1) / 2u);
                out->offset_ns = (int64_t)t3 - midpoint;
                out->status = NC_OK;
                ok = 1;
            }
        }
    }
    freeaddrinfo(res);
#if defined(_WIN32)
    WSACleanup();
#endif
    out->cpu_after = nc_clock_current_cpu();
    out->migrated = (out->cpu_before != 0xffffffffu && out->cpu_after != 0xffffffffu && out->cpu_before != out->cpu_after) ? 1u : 0u;
    return ok;
}

/* -------------------------------------------------------------------------
 * Stable clock calibration: cycles/ticks -> nanoseconds.
 * Critical path after calibration: read raw counter in ASM, subtract, divide
 * by cycles_per_ns. No kernel call, no network call, no NTP query.
 * ------------------------------------------------------------------------- */
NC_API int nc_stable_clock_default_config(nc_stable_clock_config_t *cfg) {
    if (!cfg) return 0;
    memset(cfg, 0, sizeof(*cfg));
    cfg->route = (uint32_t)NC_CLOCK_ROUTE_AUTO;
    cfg->pin_cpu = 1;
    cfg->cpu_index = 0;
    cfg->warmup_ms = 50;
    cfg->calibration_ms = 500;
    cfg->samples = 0;
    cfg->require_no_migration = 1;
    return 1;
}

NC_API const char *nc_clock_stability_advice(void) {
    return "Best precision: disable turbo/boost, lock performance governor, pin CPU affinity, avoid core migration, prefer invariant TSC or CNTVCT_EL0, isolate benchmark cores, and consider disabling deep C-states when latency matters.";
}

NC_API uint64_t nc_clock_read_raw_route(nc_ctx_t *ctx, nc_clock_route_t route) {
    if (route == NC_CLOCK_ROUTE_AUTO) route = nc_clock_select_best_route();
    return nc_clock_read_route_raw_(ctx, route);
}

static void nc_sleep_ms_stable_(uint32_t ms) {
#if defined(_WIN32)
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)(ms % 1000u) * 1000000l;
    while (nanosleep(&ts, &ts) != 0 && errno == EINTR) {}
#endif
}

NC_API int nc_stable_clock_prepare(const nc_stable_clock_config_t *cfg, nc_stable_clock_state_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    nc_stable_clock_config_t local;
    if (!cfg) { nc_stable_clock_default_config(&local); cfg = &local; }
    out->route = cfg->route == NC_CLOCK_ROUTE_AUTO ? (uint32_t)nc_clock_select_best_route() : cfg->route;
    out->cpu_before = nc_clock_current_cpu();
    if (cfg->pin_cpu) {
        out->pinned = (uint32_t)nc_clock_pin_thread_to_cpu(cfg->cpu_index);
        out->cpu_index = cfg->cpu_index;
    }
    out->cpu_after = nc_clock_current_cpu();
    out->migrated = (out->cpu_before != out->cpu_after) ? 1u : 0u;
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(_M_ARM64)
    out->invariant_hint = 1u;
#else
    out->invariant_hint = 0u;
#endif
    if (cfg->warmup_ms) nc_sleep_ms_stable_(cfg->warmup_ms);
    if (cfg->require_no_migration && out->migrated && !out->pinned) out->status = NC_ERR_UNSUPPORTED;
    return 1;
}

NC_API uint64_t nc_cycles_to_ns_calibrated(uint64_t cycles, double cycles_per_ns) {
    if (!(cycles_per_ns > 0.0)) return 0;
    long double ns = (long double)cycles / (long double)cycles_per_ns;
    if (ns < 0.0L) return 0;
    if (ns > 18446744073709551615.0L) return UINT64_MAX;
    return (uint64_t)(ns + 0.5L);
}

NC_API uint64_t nc_raw_delta_to_ns_calibrated(uint64_t raw_start, uint64_t raw_end, double units_per_ns) {
    uint64_t delta = raw_end >= raw_start ? (raw_end - raw_start) : 0;
    return nc_cycles_to_ns_calibrated(delta, units_per_ns);
}

NC_API int nc_calibrate_cycles_per_ns(nc_ctx_t *ctx, const nc_stable_clock_config_t *cfg, nc_stable_clock_state_t *out) {
    if (!out) return 0;
    nc_stable_clock_config_t local;
    if (!cfg) { nc_stable_clock_default_config(&local); cfg = &local; }
    nc_stable_clock_prepare(cfg, out);
    nc_clock_route_t route = (nc_clock_route_t)out->route;
    uint32_t ms = cfg->calibration_ms ? cfg->calibration_ms : 500u;

    uint32_t cpu0 = nc_clock_current_cpu();
    uint64_t wall0 = nc_monotonic_time_ns();
    uint64_t raw0 = nc_clock_read_raw_route(ctx, route);
    nc_sleep_ms_stable_(ms);
    uint64_t raw1 = nc_clock_read_raw_route(ctx, route);
    uint64_t wall1 = nc_monotonic_time_ns();
    uint32_t cpu1 = nc_clock_current_cpu();

    out->cpu_before = cpu0;
    out->cpu_after = cpu1;
    out->migrated = (cpu0 != cpu1) ? 1u : 0u;
    out->raw_start = raw0;
    out->raw_end = raw1;
    out->elapsed_units = raw1 >= raw0 ? (raw1 - raw0) : 0;
    out->elapsed_ns = wall1 >= wall0 ? (wall1 - wall0) : 0;
    out->cycles_per_ns = (out->elapsed_ns > 0) ? ((double)out->elapsed_units / (double)out->elapsed_ns) : 0.0;
    out->ns_per_cycle = (out->cycles_per_ns > 0.0) ? (1.0 / out->cycles_per_ns) : 0.0;
    if (ctx) {
        uint64_t a = nc_clock_read_raw_route(ctx, route);
        uint64_t b = nc_clock_read_raw_route(ctx, route);
        out->read_overhead_units = b >= a ? (b - a) : 0;
        out->kernel_timecall_overhead_cycles = nc_measure_kernel_timecall_overhead_cycles(ctx, 512);
        out->api_call_overhead_cycles = nc_measure_api_call_overhead_cycles(ctx, 512);
    }
    if (cfg->require_no_migration && out->migrated) out->status = NC_ERR_UNSUPPORTED;
    return out->cycles_per_ns > 0.0;
}

/* -------------------------------------------------------------------------
 * Cross-platform stability diagnostics: affinity validation, migration,
 * frequency drift and thermal metadata. Outside the timing hot path.
 * ------------------------------------------------------------------------- */
static void nc_diag_sleep_us_(uint32_t us) {
#if defined(_WIN32)
    DWORD ms = (DWORD)((us + 999u) / 1000u);
    if (!ms) ms = 1;
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(us / 1000000u);
    ts.tv_nsec = (long)(us % 1000000u) * 1000l;
    while (nanosleep(&ts, &ts) != 0 && errno == EINTR) {}
#endif
}

static void nc_diag_record_cpu_(uint32_t cpu, uint64_t *mask_low) {
    if (mask_low && cpu < 64u) *mask_low |= (1ull << cpu);
}

NC_API int nc_clock_detect_core_migration(uint32_t samples, uint32_t interval_us,
                                          nc_core_migration_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    if (!samples) samples = 256u;
    if (!interval_us) interval_us = 1000u;
    out->samples = samples;
    uint32_t prev = nc_clock_current_cpu();
    out->cpu_start = prev;
    out->cpu_end = prev;
    nc_diag_record_cpu_(prev, &out->observed_cpu_mask_low);
    for (uint32_t i = 0; i < samples; ++i) {
        nc_diag_sleep_us_(interval_us);
        uint32_t cur = nc_clock_current_cpu();
        nc_diag_record_cpu_(cur, &out->observed_cpu_mask_low);
        if (cur != 0xffffffffu) out->cpu_end = cur;
        if (prev != 0xffffffffu && cur != 0xffffffffu && cur != prev) {
            if (!out->migrated) out->first_migration_sample = i + 1u;
            out->migrated = 1u;
            out->migrations++;
        }
        prev = cur;
    }
    if (out->cpu_start == 0xffffffffu || out->cpu_end == 0xffffffffu) out->status = NC_ERR_UNSUPPORTED;
    return out->status == NC_OK;
}

NC_API int nc_clock_validate_cpu_pin(uint32_t cpu_index, uint32_t samples, uint32_t interval_us,
                                     nc_cpu_pin_validation_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    out->requested_cpu = cpu_index;
    out->cpu_before = nc_clock_current_cpu();
    out->pinned = (uint32_t)nc_clock_pin_thread_to_cpu(cpu_index);
    if (!samples) samples = 256u;
    if (!interval_us) interval_us = 1000u;
    out->samples = samples;
    uint32_t prev = nc_clock_current_cpu();
    nc_diag_record_cpu_(prev, &out->observed_cpu_mask_low);
    for (uint32_t i = 0; i < samples; ++i) {
        nc_diag_sleep_us_(interval_us);
        uint32_t cur = nc_clock_current_cpu();
        nc_diag_record_cpu_(cur, &out->observed_cpu_mask_low);
        if (prev != 0xffffffffu && cur != 0xffffffffu && cur != prev) {
            if (!out->migrated) out->first_migration_sample = i + 1u;
            out->migrated = 1u;
            out->migrations++;
        }
        prev = cur;
    }
    out->cpu_after = nc_clock_current_cpu();
    nc_diag_record_cpu_(out->cpu_after, &out->observed_cpu_mask_low);
    if (!out->pinned) out->status = NC_ERR_UNSUPPORTED;
    if (out->cpu_after != 0xffffffffu && out->cpu_after != cpu_index) out->migrated = 1u;
    return out->pinned ? 1 : 0;
}

NC_API int nc_clock_measure_frequency_drift(nc_ctx_t *ctx, nc_clock_route_t route,
                                            uint32_t samples, uint32_t interval_ms,
                                            nc_frequency_drift_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    if (route == NC_CLOCK_ROUTE_AUTO) route = nc_clock_select_best_route();
    if (!samples) samples = 8u;
    if (!interval_ms) interval_ms = 50u;
    out->route = (uint32_t)route;
    out->samples = samples;
    out->interval_ms = interval_ms;
    out->cpu_before = nc_clock_current_cpu();
    out->wall_start_ns = nc_monotonic_time_ns();
    out->raw_start = nc_clock_read_raw_route(ctx, route);
    uint64_t last_wall = out->wall_start_ns;
    uint64_t last_raw = out->raw_start;
    double first_ups = 0.0;
    double max_ppm_abs = 0.0;
    for (uint32_t i = 0; i < samples; ++i) {
        nc_diag_sleep_us_(interval_ms * 1000u);
        uint64_t w = nc_monotonic_time_ns();
        uint64_t r = nc_clock_read_raw_route(ctx, route);
        uint64_t dw = w >= last_wall ? (w - last_wall) : 0;
        uint64_t dr = r >= last_raw ? (r - last_raw) : 0;
        if (dw && dr) {
            double ups = (double)dr * 1.0e9 / (double)dw;
            if (first_ups == 0.0) first_ups = ups;
            else if (first_ups > 0.0) {
                double ppm = ((ups - first_ups) / first_ups) * 1000000.0;
                double abs_ppm = ppm < 0.0 ? -ppm : ppm;
                if (abs_ppm > max_ppm_abs) max_ppm_abs = abs_ppm;
            }
        }
        last_wall = w;
        last_raw = r;
    }
    out->wall_end_ns = last_wall;
    out->raw_end = last_raw;
    out->cpu_after = nc_clock_current_cpu();
    out->migrated = (out->cpu_before != 0xffffffffu && out->cpu_after != 0xffffffffu && out->cpu_before != out->cpu_after) ? 1u : 0u;
    out->elapsed_units = out->raw_end >= out->raw_start ? (out->raw_end - out->raw_start) : 0;
    out->elapsed_ns = out->wall_end_ns >= out->wall_start_ns ? (out->wall_end_ns - out->wall_start_ns) : 0;
    if (out->elapsed_ns && out->elapsed_units) {
        out->units_per_second = (double)out->elapsed_units * 1.0e9 / (double)out->elapsed_ns;
        out->ns_per_unit = (double)out->elapsed_ns / (double)out->elapsed_units;
    } else {
        out->status = NC_ERR_UNSUPPORTED;
    }
    out->ppm_error_vs_context = max_ppm_abs;
    return out->status == NC_OK;
}

#if defined(__linux__)
static int nc_contains_nocase_(const char *s, const char *needle) {
    if (!s || !needle || !needle[0]) return 0;
    size_t nl = strlen(needle);
    for (; *s; ++s) {
        size_t i = 0;
        while (i < nl) {
            char a = s[i], b = needle[i];
            if (!a) return 0;
            if (a >= 'A' && a <= 'Z') a = (char)(a - 'A' + 'a');
            if (b >= 'A' && b <= 'Z') b = (char)(b - 'A' + 'a');
            if (a != b) break;
            ++i;
        }
        if (i == nl) return 1;
    }
    return 0;
}

static int nc_read_text_file_(const char *path, char *buf, size_t cap) {
    if (!path || !buf || cap == 0) return 0;
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    size_t n = fread(buf, 1, cap - 1u, f);
    fclose(f);
    buf[n] = '\0';
    while (n && (buf[n - 1u] == '\n' || buf[n - 1u] == '\r' || buf[n - 1u] == ' ' || buf[n - 1u] == '\t')) buf[--n] = '\0';
    return n > 0;
}

static int nc_read_i32_file_(const char *path, int32_t *out) {
    char b[64];
    if (!nc_read_text_file_(path, b, sizeof(b))) return 0;
    char *end = NULL;
    long v = strtol(b, &end, 10);
    if (end == b) return 0;
    if (v > 10000000L) v = 10000000L;
    if (v < -1000000L) v = -1000000L;
    *out = (int32_t)v;
    return 1;
}
#endif

NC_API int nc_clock_query_thermal_state(nc_thermal_state_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_ERR_UNSUPPORTED;
    out->temperature_millicelsius = INT32_MIN;
#if defined(__linux__)
    DIR *d = opendir("/sys/class/thermal");
    if (!d) return 0;
    struct dirent *de;
    int found_any = 0, found_preferred = 0;
    int32_t best_temp = INT32_MIN;
    char best_source[96] = {0};
    while ((de = readdir(d)) != NULL) {
        if (strncmp(de->d_name, "thermal_zone", 12) != 0) continue;
        char base[256], type_path[320], temp_path[320], type[96];
        const char thermal_prefix[] = "/sys/class/thermal/";
        size_t prefix_len = sizeof(thermal_prefix) - 1u;
        size_t name_len = strlen(de->d_name);
        if (name_len >= sizeof(base) - prefix_len) continue;
        memcpy(base, thermal_prefix, prefix_len);
        memcpy(base + prefix_len, de->d_name, name_len + 1u);
        snprintf(type_path, sizeof(type_path), "%s/type", base);
        snprintf(temp_path, sizeof(temp_path), "%s/temp", base);
        type[0] = '\0';
        nc_read_text_file_(type_path, type, sizeof(type));
        int32_t temp = 0;
        if (!nc_read_i32_file_(temp_path, &temp)) continue;
        found_any = 1;
        int preferred = nc_contains_nocase_(type, "cpu") || nc_contains_nocase_(type, "soc") || nc_contains_nocase_(type, "ap") || nc_contains_nocase_(type, "cluster");
        if ((!found_preferred && preferred) || (preferred == found_preferred && temp > best_temp) || (!found_preferred && best_temp == INT32_MIN)) {
            if (preferred) found_preferred = 1;
            best_temp = temp;
            if (type[0]) {
                snprintf(best_source, sizeof(best_source), "%.47s:%.47s", de->d_name, type);
            } else {
                snprintf(best_source, sizeof(best_source), "%.95s", de->d_name);
            }
        }
    }
    closedir(d);
    if (found_any) {
        out->status = NC_OK;
        out->source_available = 1u;
        out->temperature_millicelsius = best_temp;
        snprintf(out->source, sizeof(out->source), "%s", best_source[0] ? best_source : "thermal_zone");
        out->throttling_detected = (best_temp >= 85000) ? 1u : 0u;
        return 1;
    }
#else
    snprintf(out->source, sizeof(out->source), "%s", "unsupported");
#endif
    return 0;
}

NC_API int nc_clock_system_stability_snapshot(nc_ctx_t *ctx, uint32_t cpu_index,
                                              nc_system_stability_snapshot_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->status = NC_OK;
    nc_clock_validate_cpu_pin(cpu_index, 64u, 1000u, &out->pin);
    nc_clock_detect_core_migration(64u, 1000u, &out->migration);
    nc_clock_measure_frequency_drift(ctx, NC_CLOCK_ROUTE_AUTO, 6u, 50u, &out->drift);
    nc_clock_query_thermal_state(&out->thermal);
    if (out->pin.status != NC_OK || out->migration.status != NC_OK || out->drift.status != NC_OK) out->status = NC_ERR_UNSUPPORTED;
    return 1;
}
