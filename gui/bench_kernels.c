/* =============================================================================
 * bench_kernels.c - scalar/MMX/SSE-family/AES/PCLMUL/SHA CPU benchmark kernels
 *
 * This file intentionally avoids AVX codegen. AVX-family kernels live in
 * bench_kernels_avx2.c so the generic GUI/CLI can start on older CPUs and only
 * dispatch AVX code after CPUID + XGETBV checks.
 * ============================================================================= */
#define WIN32_LEAN_AND_MEAN
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#if defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
#  define NC_BENCH_X86 1
#else
#  define NC_BENCH_X86 0
#endif

#if NC_BENCH_X86
#  include <intrin.h>
#  if defined(__GNUC__) || defined(__clang__)
#    include <cpuid.h>
#  endif
#  include <xmmintrin.h>
#  include <emmintrin.h>
#  include <pmmintrin.h>
#  include <tmmintrin.h>
#  include <smmintrin.h>
#  include <nmmintrin.h>
#  include <wmmintrin.h>
#endif
#include "bench_kernels.h"

#ifndef NC_NOINLINE
#  if defined(_MSC_VER) || defined(_WIN32)
#    define NC_NOINLINE __declspec(noinline)
#  elif defined(__GNUC__) || defined(__clang__)
#    define NC_NOINLINE __attribute__((noinline))
#  else
#    define NC_NOINLINE
#  endif
#endif

#ifndef NC_HAS_SHA_INTRIN
#  if NC_BENCH_X86 && defined(__has_include)
#    if __has_include(<immintrin.h>)
#      include <immintrin.h>
#      define NC_HAS_SHA_INTRIN 1
#    else
#      define NC_HAS_SHA_INTRIN 0
#    endif
#  else
#    define NC_HAS_SHA_INTRIN 0
#  endif
#endif

static void nc_bench_cpuid(int out[4], uint32_t leaf, uint32_t subleaf) {
    out[0] = out[1] = out[2] = out[3] = 0;
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    __cpuidex(out, (int)leaf, (int)subleaf);
#elif (defined(__GNUC__) || defined(__clang__)) && (defined(__x86_64__) || defined(__i386__))
    unsigned int a = 0, b = 0, c = 0, d = 0;
    if (__get_cpuid_count(leaf, subleaf, &a, &b, &c, &d)) {
        out[0] = (int)a;
        out[1] = (int)b;
        out[2] = (int)c;
        out[3] = (int)d;
    }
#else
    (void)leaf;
    (void)subleaf;
#endif
}

static uint64_t nc_xgetbv0_safe(void) {
#if defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
    int regs[4] = {0,0,0,0};
    nc_bench_cpuid(regs, 1, 0);
    if (!((uint32_t)regs[2] & (1u << 27))) return 0; /* OSXSAVE */
#  if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    return _xgetbv(0);
#  elif (defined(__GNUC__) || defined(__clang__)) && (defined(__x86_64__) || defined(__i386__))
    uint32_t eax = 0, edx = 0;
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
    return ((uint64_t)edx << 32) | eax;
#  else
    return 0;
#  endif
#else
    return 0;
#endif
}

void nc_bench_get_cpu_features(nc_bench_cpu_features_t *out) {
    if (!out) return;
    memset(out, 0, sizeof(*out));
#if defined(_M_X64) || defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
    int r1[4] = {0,0,0,0};
    int r7[4] = {0,0,0,0};
    int max_leaf_regs[4] = {0,0,0,0};
    nc_bench_cpuid(max_leaf_regs, 0, 0);
    const uint32_t max_leaf = (uint32_t)max_leaf_regs[0];
    if (max_leaf < 1) return;

    nc_bench_cpuid(r1, 1, 0);
    const uint32_t ecx1 = (uint32_t)r1[2];
    const uint32_t edx1 = (uint32_t)r1[3];
    const uint64_t xcr0 = nc_xgetbv0_safe();
    const int os_avx = ((xcr0 & 0x6u) == 0x6u);
    const int os_avx512 = ((xcr0 & 0xE6u) == 0xE6u);

    out->mmx       = (edx1 & (1u << 23)) != 0;
    out->sse       = (edx1 & (1u << 25)) != 0;
    out->sse2      = (edx1 & (1u << 26)) != 0;
    out->sse3      = (ecx1 & (1u << 0))  != 0;
    out->pclmulqdq = (ecx1 & (1u << 1))  != 0;
    out->ssse3     = (ecx1 & (1u << 9))  != 0;
    out->fma       = ((ecx1 & (1u << 12)) != 0) && os_avx;
    out->sse41     = (ecx1 & (1u << 19)) != 0;
    out->sse42     = (ecx1 & (1u << 20)) != 0;
    out->aesni     = (ecx1 & (1u << 25)) != 0;
    out->avx       = ((ecx1 & (1u << 28)) != 0) && os_avx;
    out->f16c      = ((ecx1 & (1u << 29)) != 0) && os_avx;

    if (max_leaf >= 7) {
        nc_bench_cpuid(r7, 7, 0);
        const uint32_t max_subleaf7 = (uint32_t)r7[0];
        const uint32_t ebx7 = (uint32_t)r7[1];
        const uint32_t ecx7 = (uint32_t)r7[2];
        out->avx2       = ((ebx7 & (1u << 5))  != 0) && os_avx;
        out->shani      = (ebx7 & (1u << 29)) != 0;
        out->avx512f    = ((ebx7 & (1u << 16)) != 0) && os_avx512;
        out->avx512bw   = ((ebx7 & (1u << 30)) != 0) && os_avx512;
        out->avx512vl   = ((ebx7 & (1u << 31)) != 0) && os_avx512;
        out->vaes       = ((ecx7 & (1u << 9))  != 0) && os_avx;
        out->avx512vnni = ((ecx7 & (1u << 11)) != 0) && os_avx512;

        if (max_subleaf7 >= 1) {
            int r71[4] = {0,0,0,0};
            nc_bench_cpuid(r71, 7, 1);
            const uint32_t eax71 = (uint32_t)r71[0];
            out->avx_vnni = ((eax71 & (1u << 4)) != 0) && os_avx && out->avx2;
        }
    }
#else
#  if defined(_M_X64) || defined(__x86_64__)
    out->sse = 1;
    out->sse2 = 1;
#  endif
#endif
}
int nc_bench_kernel_available(nc_bench_kernel_id_t id, const nc_bench_cpu_features_t *f) {
    nc_bench_cpu_features_t local;
    if (!f) { nc_bench_get_cpu_features(&local); f = &local; }
    switch (id) {
    case NC_BENCH_ASM_LEGACY: return NC_BENCH_X86;
    case NC_BENCH_MMX:        return f->mmx;
    case NC_BENCH_SSE:        return f->sse;
    case NC_BENCH_SSE2:       return f->sse2;
    case NC_BENCH_SSE3:       return f->sse3;
    case NC_BENCH_SSSE3:      return f->ssse3;
    case NC_BENCH_SSE41:      return f->sse41;
    case NC_BENCH_SSE42:      return f->sse42;
    case NC_BENCH_AESNI:      return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_PCLMULQDQ:  return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_SHANI:      return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_AVX:        return f->avx;
    case NC_BENCH_F16C:       return f->f16c;
    case NC_BENCH_FMA:        return f->fma;
    case NC_BENCH_AVX2:       return f->avx2;
    case NC_BENCH_AVX_VNNI:  return f->avx_vnni;
    case NC_BENCH_VAES:       return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_AVX512F:    return f->avx512f;
    case NC_BENCH_AVX512BW:   return f->avx512bw;
    case NC_BENCH_AVX512VL:   return f->avx512vl;
    case NC_BENCH_AVX512VNNI: return f->avx512vnni;
    default: return 0;
    }
}

const char *nc_bench_kernel_name(nc_bench_kernel_id_t id) {
    switch (id) {
    case NC_BENCH_ASM_LEGACY: return "Legacy scalar ASM";
    case NC_BENCH_MMX:        return "MMX";
    case NC_BENCH_SSE:        return "SSE";
    case NC_BENCH_SSE2:       return "SSE2";
    case NC_BENCH_SSE3:       return "SSE3";
    case NC_BENCH_SSSE3:      return "SSSE3";
    case NC_BENCH_SSE41:      return "SSE4.1";
    case NC_BENCH_SSE42:      return "SSE4.2";
    case NC_BENCH_AESNI:      return "AES-NI";
    case NC_BENCH_PCLMULQDQ:  return "PCLMULQDQ";
    case NC_BENCH_SHANI:      return "SHA-NI";
    case NC_BENCH_AVX:        return "AVX";
    case NC_BENCH_F16C:       return "F16C";
    case NC_BENCH_FMA:        return "FMA";
    case NC_BENCH_AVX2:       return "AVX2";
    case NC_BENCH_AVX_VNNI:  return "AVX-VNNI";
    case NC_BENCH_VAES:       return "VAES";
    case NC_BENCH_AVX512F:    return "AVX-512";
    case NC_BENCH_AVX512BW:   return "AVX-512BW";
    case NC_BENCH_AVX512VL:   return "AVX-512VL";
    case NC_BENCH_AVX512VNNI: return "AVX-512VNNI";
    default: return "unknown";
    }
}

const char *nc_bench_kernel_short_name(nc_bench_kernel_id_t id) {
    switch (id) {
    case NC_BENCH_ASM_LEGACY: return "legacy";
    case NC_BENCH_MMX:        return "mmx";
    case NC_BENCH_SSE:        return "sse";
    case NC_BENCH_SSE2:       return "sse2";
    case NC_BENCH_SSE3:       return "sse3";
    case NC_BENCH_SSSE3:      return "ssse3";
    case NC_BENCH_SSE41:      return "sse41";
    case NC_BENCH_SSE42:      return "sse42";
    case NC_BENCH_AESNI:      return "aesni";
    case NC_BENCH_PCLMULQDQ:  return "pclmulqdq";
    case NC_BENCH_SHANI:      return "shani";
    case NC_BENCH_AVX:        return "avx";
    case NC_BENCH_F16C:       return "f16c";
    case NC_BENCH_FMA:        return "fma";
    case NC_BENCH_AVX2:       return "avx2";
    case NC_BENCH_AVX_VNNI:  return "avx-vnni";
    case NC_BENCH_VAES:       return "vaes";
    case NC_BENCH_AVX512F:    return "avx512";
    case NC_BENCH_AVX512BW:   return "avx512bw";
    case NC_BENCH_AVX512VL:   return "avx512vl";
    case NC_BENCH_AVX512VNNI: return "avx512vnni";
    default: return "unknown";
    }
}

#if NC_BENCH_X86
NC_NOINLINE uint64_t nc_bench_kernel_sse(size_t loops) {
    __m128 a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
    __m128 b = _mm_set1_ps(1.00024414f);
    __m128 c = _mm_set1_ps(0.99975586f);
    __m128 acc = _mm_setzero_ps();
    for (size_t i = 0; i < loops; ++i) {
        acc = _mm_add_ps(acc, _mm_mul_ps(a, b));
        a = _mm_add_ps(a, c);
        b = _mm_mul_ps(b, _mm_set1_ps(1.00000012f));
    }
    float out[4];
    _mm_storeu_ps(out, acc);
    uint64_t s = 0;
    for (int i = 0; i < 4; ++i) { union { float f; uint32_t u; } v; v.f = out[i]; s ^= (uint64_t)v.u << ((i & 1) ? 32 : 0); }
    return s;
}

NC_NOINLINE uint64_t nc_bench_kernel_sse2(size_t loops) {
    __m128i x0 = _mm_set_epi32(1,2,3,4);
    __m128i x1 = _mm_set_epi32(5,6,7,8);
    __m128i m = _mm_set1_epi32(0x9E3779B9u);
    for (size_t i = 0; i < loops; ++i) {
        x0 = _mm_add_epi32(x0, m);
        x1 = _mm_xor_si128(x1, x0);
        x0 = _mm_shuffle_epi32(x0, _MM_SHUFFLE(2,1,0,3));
        x1 = _mm_add_epi64(x1, _mm_srli_epi64(x0, 7));
    }
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, _mm_xor_si128(x0, x1));
    return out[0] ^ out[1];
}

NC_NOINLINE uint64_t nc_bench_kernel_sse3(size_t loops) {
    __m128 x = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
    __m128 y = _mm_set_ps(8.0f, 7.0f, 6.0f, 5.0f);
    for (size_t i = 0; i < loops; ++i) {
        x = _mm_add_ps(x, y);
        y = _mm_hadd_ps(y, x);
        x = _mm_hadd_ps(x, y);
    }
    float out[4];
    _mm_storeu_ps(out, _mm_add_ps(x, y));
    uint64_t s = 0;
    for (int i = 0; i < 4; ++i) { union { float f; uint32_t u; } v; v.f = out[i]; s ^= (uint64_t)v.u << ((i & 1) ? 32 : 0); }
    return s;
}

NC_NOINLINE uint64_t nc_bench_kernel_ssse3(size_t loops) {
    __m128i x = _mm_set_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    __m128i mask = _mm_set_epi8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    __m128i acc = _mm_setzero_si128();
    for (size_t i = 0; i < loops; ++i) {
        x = _mm_shuffle_epi8(x, mask);
        acc = _mm_add_epi8(acc, x);
        mask = _mm_xor_si128(mask, acc);
    }
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, _mm_xor_si128(acc, x));
    return out[0] ^ out[1];
}

NC_NOINLINE uint64_t nc_bench_kernel_sse41(size_t loops) {
    __m128i x = _mm_set_epi32(1,2,3,4);
    __m128i y = _mm_set1_epi32(0x7F4A7C15u);
    __m128i acc = _mm_setzero_si128();
    for (size_t i = 0; i < loops; ++i) {
        x = _mm_mullo_epi32(x, y);
        acc = _mm_blend_epi16(acc, x, 0xAA);
        y = _mm_add_epi32(y, _mm_set1_epi32(0x9E3779B9u));
    }
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, _mm_xor_si128(acc, x));
    return out[0] ^ out[1];
}

NC_NOINLINE uint64_t nc_bench_kernel_sse42(size_t loops) {
#if defined(_M_X64) || defined(__x86_64__)
    uint64_t crc = 0xD1B54A32D192ED03ull;
    uint64_t x = 0x0123456789ABCDEFull;
    for (size_t i = 0; i < loops; ++i) {
        crc = _mm_crc32_u64(crc, x);
        x += 0x9E3779B97F4A7C15ull + crc;
        crc = _mm_crc32_u64(crc, x ^ (x >> 17));
    }
    return crc ^ x;
#else
    uint32_t crc = 0xD192ED03u;
    uint32_t x = 0x89ABCDEFu;
    for (size_t i = 0; i < loops; ++i) {
        crc = _mm_crc32_u32(crc, x);
        x += 0x9E3779B9u + crc;
    }
    return ((uint64_t)crc << 32) | x;
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_aesni(size_t loops) {
    __m128i rk = _mm_set_epi64x(0x0F1E2D3C4B5A6978ll, 0x8796A5B4C3D2E1F0ll);
    __m128i x0 = _mm_set_epi64x(0x0011223344556677ll, 0x8899AABBCCDDEEFFll);
    __m128i x1 = _mm_xor_si128(x0, _mm_set1_epi32(0x13579BDF));
    __m128i x2 = _mm_add_epi64(x1, _mm_set1_epi32(0x2468ACE0));
    __m128i x3 = _mm_xor_si128(x2, _mm_set_epi32(1,2,3,4));
    for (size_t i = 0; i < loops; ++i) {
        x0 = _mm_aesenc_si128(x0, rk);
        x1 = _mm_aesenc_si128(x1, rk);
        x2 = _mm_aesenc_si128(x2, rk);
        x3 = _mm_aesenc_si128(x3, rk);
        x0 = _mm_aesenclast_si128(x0, rk);
        x1 = _mm_aesenclast_si128(x1, rk);
        x2 = _mm_aesenclast_si128(x2, rk);
        x3 = _mm_aesenclast_si128(x3, rk);
    }
    x0 = _mm_xor_si128(_mm_xor_si128(x0, x1), _mm_xor_si128(x2, x3));
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, x0);
    return out[0] ^ out[1];
}

NC_NOINLINE uint64_t nc_bench_kernel_pclmul(size_t loops) {
    __m128i a = _mm_set_epi64x(0x0123456789ABCDEFull, 0xFEDCBA9876543210ull);
    __m128i b = _mm_set_epi64x(0x0F0E0D0C0B0A0908ll, 0x0706050403020100ll);
    __m128i acc = _mm_setzero_si128();
    for (size_t i = 0; i < loops; ++i) {
        __m128i p0 = _mm_clmulepi64_si128(a, b, 0x00);
        __m128i p1 = _mm_clmulepi64_si128(a, b, 0x11);
        acc = _mm_xor_si128(acc, _mm_xor_si128(p0, p1));
        a = _mm_xor_si128(a, _mm_slli_epi64(acc, 1));
        b = _mm_xor_si128(b, _mm_srli_epi64(acc, 1));
    }
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, acc);
    return out[0] ^ out[1];
}

#if NC_HAS_SHA_INTRIN
NC_NOINLINE uint64_t nc_bench_kernel_shani(size_t loops) {
    __m128i s0 = _mm_set_epi32(0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A);
    __m128i s1 = _mm_set_epi32(0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19);
    __m128i msg = _mm_set_epi32(0,1,2,3);
    for (size_t i = 0; i < loops; ++i) {
        __m128i t0 = _mm_sha256msg1_epu32(msg, s0);
        __m128i t1 = _mm_sha256msg2_epu32(t0, s1);
        s1 = _mm_sha256rnds2_epu32(s1, s0, t1);
        s0 = _mm_sha256rnds2_epu32(s0, s1, t0);
        msg = _mm_add_epi32(msg, _mm_set1_epi32(0x9E3779B9));
    }
    uint64_t out[2];
    _mm_storeu_si128((__m128i*)out, _mm_xor_si128(s0, s1));
    return out[0] ^ out[1];
}
#else
NC_NOINLINE uint64_t nc_bench_kernel_shani(size_t loops) { return nc_bench_kernel_sse2(loops); }
#endif


#else
static uint64_t nc_bench_non_x86_stub(size_t loops) {
    uint64_t x = 0x9E3779B97F4A7C15ull;
    for (size_t i = 0; i < loops; ++i) {
        x ^= x << 7;
        x ^= x >> 9;
        x += 0xD1B54A32D192ED03ull;
    }
    return x;
}
NC_NOINLINE uint64_t nc_bench_kernel_asm_legacy(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_mmx(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_sse(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_sse2(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_sse3(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_ssse3(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_sse41(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_sse42(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_aesni(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_pclmul(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_shani(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_f16c(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_fma(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx2(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx_vnni(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_vaes(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx512(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx512bw(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx512vl(size_t loops) { return nc_bench_non_x86_stub(loops); }
NC_NOINLINE uint64_t nc_bench_kernel_avx512vnni(size_t loops) { return nc_bench_non_x86_stub(loops); }
#endif

uint64_t nc_bench_run_cpu_kernel(nc_bench_kernel_id_t id, size_t loops) {
    switch (id) {
    case NC_BENCH_ASM_LEGACY: return nc_bench_kernel_asm_legacy(loops);
    case NC_BENCH_MMX:        return nc_bench_kernel_mmx(loops);
    case NC_BENCH_SSE:        return nc_bench_kernel_sse(loops);
    case NC_BENCH_SSE2:       return nc_bench_kernel_sse2(loops);
    case NC_BENCH_SSE3:       return nc_bench_kernel_sse3(loops);
    case NC_BENCH_SSSE3:      return nc_bench_kernel_ssse3(loops);
    case NC_BENCH_SSE41:      return nc_bench_kernel_sse41(loops);
    case NC_BENCH_SSE42:      return nc_bench_kernel_sse42(loops);
    case NC_BENCH_AESNI:
    case NC_BENCH_PCLMULQDQ:
    case NC_BENCH_SHANI:
        return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_AVX:        return nc_bench_kernel_avx(loops);
    case NC_BENCH_F16C:       return nc_bench_kernel_f16c(loops);
    case NC_BENCH_FMA:        return nc_bench_kernel_fma(loops);
    case NC_BENCH_AVX2:       return nc_bench_kernel_avx2(loops);
    case NC_BENCH_AVX_VNNI:  return nc_bench_kernel_avx_vnni(loops);
    case NC_BENCH_VAES:       return 0; /* crypto is delegated to OpenSSL/libsodium */
    case NC_BENCH_AVX512F:    return nc_bench_kernel_avx512(loops);
    case NC_BENCH_AVX512BW:   return nc_bench_kernel_avx512bw(loops);
    case NC_BENCH_AVX512VL:   return nc_bench_kernel_avx512vl(loops);
    case NC_BENCH_AVX512VNNI: return nc_bench_kernel_avx512vnni(loops);
    default: return nc_bench_kernel_asm_legacy(loops);
    }
}
