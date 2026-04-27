/* ARM64 compatibility shims for legacy x86/TSC/AVX public symbols.
 *
 * The original Win32 GUI and runtime still call a few legacy x64 names
 * (nc_tsc_*, nc_avx_*) even when the target architecture is ARM64.  ARM64
 * does not have RDTSC/AVX/SSE, so these wrappers provide safe fallbacks:
 *   - timer values come from QueryPerformanceCounter on Windows;
 *   - AVX/SSE feature checks return false;
 *   - SIMD memory helpers fall back to portable C memset/memcpy loops.
 */
#include "nanochrono.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif

static uint64_t nc_arm64_compat_ticks(void) {
#if defined(_WIN32)
    LARGE_INTEGER v;
    QueryPerformanceCounter(&v);
    return (uint64_t)v.QuadPart;
#else
    return 0;
#endif
}

NC_API uint64_t nc_tsc_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_raw(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_lfence_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_lfence_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_mfence_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_mfence_end(void) { return nc_arm64_compat_ticks(); }
NC_API void nc_tsc_serialize(void) { }
NC_API uint32_t nc_tsc_aux(void) { return 0; }
NC_API void nc_tsc_rdtscp_pair(nc_rdtscp_pair_t *out) { if (out) { out->tsc = nc_arm64_compat_ticks(); out->aux = 0; out->_pad = 0; } }
NC_API uint64_t nc_tsc_delta(uint64_t start, uint64_t end) { return end - start; }
NC_API uint64_t nc_tsc_elapsed_from(uint64_t start) { return nc_arm64_compat_ticks() - start; }
NC_API uint64_t nc_tsc_rdtscp_elapsed(uint64_t start, uint32_t *aux_out) { if (aux_out) *aux_out = 0; return nc_arm64_compat_ticks() - start; }
NC_API void nc_tsc_spin_until(uint64_t target_cycles) { while (nc_arm64_compat_ticks() < target_cycles) { } }
NC_API uint64_t nc_tsc_overhead(void) { uint64_t a = nc_arm64_compat_ticks(); uint64_t b = nc_arm64_compat_ticks(); return b - a; }
NC_API int nc_tsc_invariant(void) { return 1; }
NC_API void nc_tsc_cpu_brand(char buf[49]) { if (buf) { strncpy(buf, "ARM64 Windows", 48); buf[48] = '\0'; } }
NC_API uint32_t nc_tsc_max_leaf(void) { return 0; }

NC_API void nc_simd_zero_buf(void *dst, size_t bytes) { if (dst && bytes) memset(dst, 0, bytes); }
NC_API void nc_simd_copy_buf(void *dst, const void *src, size_t bytes) { if (dst && src && bytes) memcpy(dst, src, bytes); }
NC_API uint64_t nc_simd_xor_checksum(const void *ptr, size_t bytes) { const unsigned char *p = (const unsigned char *)ptr; uint64_t x = 0; if (!p) return 0; for (size_t i = 0; i < bytes; ++i) x ^= p[i]; return x; }
NC_API uint64_t nc_simd_sum_u64(const uint64_t *ptr, size_t count) { uint64_t s = 0; if (!ptr) return 0; for (size_t i = 0; i < count; ++i) s += ptr[i]; return s; }
NC_API void nc_simd_nt_zero(void *dst_aligned, size_t bytes) { nc_simd_zero_buf(dst_aligned, bytes); }
NC_API void nc_simd_prefetch_loop(const void *ptr, size_t bytes, size_t stride) { (void)ptr; (void)bytes; (void)stride; }
NC_API uint64_t nc_tsc_prefetch_start(const void *prefetch_ptr) { (void)prefetch_ptr; return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_prefetch_end(const void *prefetch_ptr) { (void)prefetch_ptr; return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_pause_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_tsc_full_fence(void) { return nc_arm64_compat_ticks(); }
NC_API void nc_tsc_rdtscp_seq(nc_rdtscp_seq_t *stamp) { if (stamp) { stamp->tsc = nc_arm64_compat_ticks(); stamp->aux = 0; stamp->seq++; } }
NC_API uint64_t tsc_start_legacy(void) { return nc_tsc_start(); }
NC_API uint64_t tsc_end_legacy(void) { return nc_tsc_end(); }

NC_API uint64_t nc_avx_tsc_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_raw(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_full_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_full_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_upper_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_upper_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_sfence_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_sfence_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_mfence_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_mfence_end(void) { return nc_arm64_compat_ticks(); }
NC_API uint32_t nc_avx_tsc_aux(void) { return 0; }
NC_API void nc_avx_tsc_rdtscp_pair(nc_rdtscp_pair_t *out) { nc_tsc_rdtscp_pair(out); }
NC_API uint64_t nc_avx_tsc_delta(uint64_t start, uint64_t end) { return end - start; }
NC_API uint64_t nc_avx_tsc_elapsed_from(uint64_t start) { return nc_arm64_compat_ticks() - start; }
NC_API void nc_avx_spin_until(uint64_t target_cycles) { nc_tsc_spin_until(target_cycles); }
NC_API uint64_t nc_avx_tsc_overhead(void) { return nc_tsc_overhead(); }
NC_API int nc_avx_tsc_invariant(void) { return 0; }
NC_API void nc_avx_vzeroall(void) { }
NC_API void nc_avx_warmup(void) { }
NC_API void nc_avx_simd_zero_buf(void *dst, size_t bytes) { nc_simd_zero_buf(dst, bytes); }
NC_API void nc_avx_simd_copy_buf(void *dst, const void *src, size_t bytes) { nc_simd_copy_buf(dst, src, bytes); }
NC_API uint64_t nc_avx_simd_xor_checksum(const void *ptr, size_t bytes) { return nc_simd_xor_checksum(ptr, bytes); }
NC_API uint64_t nc_avx_simd_sum_u64(const uint64_t *ptr, size_t count) { return nc_simd_sum_u64(ptr, count); }
NC_API void nc_avx_simd_nt_zero(void *dst_aligned32, size_t bytes) { nc_simd_zero_buf(dst_aligned32, bytes); }
NC_API void nc_avx_simd_prefetch_loop(const void *ptr, size_t bytes, size_t stride) { (void)ptr; (void)bytes; (void)stride; }
NC_API uint64_t nc_avx_prefetch_tsc_start(const void *prefetch_ptr) { (void)prefetch_ptr; return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_prefetch_tsc_end(const void *prefetch_ptr) { (void)prefetch_ptr; return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_pause_start(void) { return nc_arm64_compat_ticks(); }
NC_API uint64_t nc_avx_tsc_full_fence(void) { return nc_arm64_compat_ticks(); }
NC_API void nc_avx_tsc_rdtscp_seq(nc_rdtscp_seq_t *stamp) { nc_tsc_rdtscp_seq(stamp); }
NC_API int nc_avx_check(void) { return 0; }
NC_API int nc_avx2_check(void) { return 0; }
NC_API uint64_t tsc_start_simd(void) { return nc_avx_tsc_start(); }
NC_API uint64_t tsc_end_simd(void) { return nc_avx_tsc_end(); }
