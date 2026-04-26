/* =============================================================================
 * bench_kernels_avx2.c - AVX/F16C/FMA/AVX2/AVX512 timer-family benchmark kernels
 * ============================================================================= */
#define WIN32_LEAN_AND_MEAN
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>
#include <immintrin.h>
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

NC_NOINLINE uint64_t nc_bench_kernel_vaes(size_t loops) {
#if defined(__VAES__)
    __m256i rk = _mm256_set1_epi64x(0x0F1E2D3C4B5A6978ll);
    __m256i x0 = _mm256_set_epi64x(1,2,3,4);
    __m256i x1 = _mm256_set_epi64x(5,6,7,8);
    for (size_t i = 0; i < loops; ++i) {
        x0 = _mm256_aesenc_epi128(x0, rk);
        x1 = _mm256_aesenc_epi128(x1, rk);
        x0 = _mm256_aesenclast_epi128(x0, rk);
        x1 = _mm256_aesenclast_epi128(x1, rk);
    }
    __m256i x = _mm256_xor_si256(x0, x1);
    uint64_t out[4];
    _mm256_storeu_si256((__m256i*)out, x);
    return out[0] ^ out[1] ^ out[2] ^ out[3];
#else
    return nc_bench_kernel_aesni(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_avx(size_t loops) {
    __m256 a0 = _mm256_set_ps(8.0f,7.0f,6.0f,5.0f,4.0f,3.0f,2.0f,1.0f);
    __m256 a1 = _mm256_set_ps(1.5f,2.5f,3.5f,4.5f,5.5f,6.5f,7.5f,8.5f);
    __m256 b0 = _mm256_set1_ps(1.00097656f);
    __m256 b1 = _mm256_set1_ps(0.99951172f);
    __m256 acc0 = _mm256_setzero_ps();
    __m256 acc1 = _mm256_set1_ps(0.5f);
    for (size_t i = 0; i < loops; ++i) {
        acc0 = _mm256_add_ps(acc0, _mm256_mul_ps(a0, b0));
        acc1 = _mm256_add_ps(acc1, _mm256_mul_ps(a1, b1));
        a0 = _mm256_add_ps(a0, _mm256_set1_ps(0.0001f));
        a1 = _mm256_sub_ps(a1, _mm256_set1_ps(0.0002f));
        b0 = _mm256_mul_ps(b0, _mm256_set1_ps(1.00000012f));
        b1 = _mm256_mul_ps(b1, _mm256_set1_ps(0.99999988f));
    }
    __m256 x = _mm256_xor_ps(acc0, acc1);
    float out[8];
    _mm256_storeu_ps(out, x);
    uint64_t s = 0;
    for (int i = 0; i < 8; ++i) {
        union { float f; uint32_t u; } v;
        v.f = out[i];
        s ^= ((uint64_t)v.u << ((i & 1) ? 32 : 0));
        s = (s << 9) | (s >> 55);
    }
    return s;
}

NC_NOINLINE uint64_t nc_bench_kernel_avx2(size_t loops) {
    __m256i x0 = _mm256_set_epi32(1,2,3,4,5,6,7,8);
    __m256i x1 = _mm256_set_epi32(9,10,11,12,13,14,15,16);
    __m256i m0 = _mm256_set1_epi32(0x9E3779B9u);
    __m256i m1 = _mm256_set1_epi32(0x7F4A7C15u);
    for (size_t i = 0; i < loops; ++i) {
        x0 = _mm256_add_epi32(x0, m0);
        x1 = _mm256_xor_si256(x1, x0);
        x0 = _mm256_mullo_epi32(x0, m1);
        x1 = _mm256_add_epi32(x1, _mm256_srli_epi32(x0, 13));
    }
    __m256i x = _mm256_xor_si256(x0, x1);
    uint32_t out[8];
    _mm256_storeu_si256((__m256i*)out, x);
    uint64_t s = 0;
    for (int i = 0; i < 8; ++i) {
        s ^= ((uint64_t)out[i] << ((i & 1) ? 32 : 0));
        s = (s << 7) | (s >> 57);
    }
    return s;
}

NC_NOINLINE uint64_t nc_bench_kernel_avx512(size_t loops) {
#if defined(__AVX512F__)
    __m512 a = _mm512_set_ps(16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1);
    __m512 b = _mm512_set1_ps(1.00024414f);
    __m512 acc = _mm512_setzero_ps();
    for (size_t i = 0; i < loops; ++i) {
        acc = _mm512_add_ps(acc, _mm512_mul_ps(a, b));
        a = _mm512_add_ps(a, _mm512_set1_ps(0.00005f));
        b = _mm512_mul_ps(b, _mm512_set1_ps(0.99999994f));
    }
    float out[16];
    _mm512_storeu_ps(out, acc);
    uint64_t s = 0;
    for (int i = 0; i < 16; ++i) {
        union { float f; uint32_t u; } v;
        v.f = out[i];
        s ^= ((uint64_t)v.u << ((i & 1) ? 32 : 0));
        s = (s << 11) | (s >> 53);
    }
    return s;
#else
    return nc_bench_kernel_avx2(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_f16c(size_t loops) {
#if defined(_MSC_VER) || defined(__F16C__)
    __m256 x = _mm256_set_ps(8.0f,7.0f,6.0f,5.0f,4.0f,3.0f,2.0f,1.0f);
    __m128i h = _mm_setzero_si128();
    for (size_t i = 0; i < loops; ++i) {
        h = _mm256_cvtps_ph(x, 0);
        x = _mm256_add_ps(_mm256_cvtph_ps(h), _mm256_set1_ps(0.0009765625f));
    }
    uint16_t out[8];
    _mm_storeu_si128((__m128i*)out, h);
    uint64_t s = 0;
    for (int i = 0; i < 8; ++i) s = (s << 7) ^ out[i] ^ (s >> 3);
    return s;
#else
    return nc_bench_kernel_avx(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_fma(size_t loops) {
#if defined(_MSC_VER) || defined(__FMA__)
    __m256 a = _mm256_set1_ps(1.00024414f);
    __m256 b = _mm256_set_ps(8,7,6,5,4,3,2,1);
    __m256 c = _mm256_set1_ps(0.99975586f);
    for (size_t i = 0; i < loops; ++i) {
        a = _mm256_fmadd_ps(a, b, c);
        b = _mm256_fmadd_ps(b, c, a);
        c = _mm256_fnmadd_ps(c, a, b);
    }
    float out[8];
    _mm256_storeu_ps(out, _mm256_xor_ps(a, b));
    uint64_t s = 0;
    for (int i = 0; i < 8; ++i) { union { float f; uint32_t u; } v; v.f = out[i]; s ^= (uint64_t)v.u << ((i & 1) ? 32 : 0); }
    return s;
#else
    return nc_bench_kernel_avx(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_avx512bw(size_t loops) {
#if defined(__AVX512BW__) && defined(__AVX512F__)
    __m512i x = _mm512_set1_epi16(0x1234);
    __m512i y = _mm512_set1_epi16(0x0101);
    for (size_t i = 0; i < loops; ++i) {
        x = _mm512_add_epi16(x, y);
        y = _mm512_xor_si512(y, x);
    }
    uint64_t out[8];
    _mm512_storeu_si512((__m512i*)out, x);
    return out[0]^out[1]^out[2]^out[3]^out[4]^out[5]^out[6]^out[7];
#else
    return nc_bench_kernel_avx512(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_avx512vl(size_t loops) {
#if defined(__AVX512VL__) && defined(__AVX512F__)
    __m256 x = _mm256_set_ps(8,7,6,5,4,3,2,1);
    __mmask8 k = 0xAA;
    for (size_t i = 0; i < loops; ++i) {
        x = _mm256_mask_add_ps(x, k, x, _mm256_set1_ps(1.0f));
        k = (unsigned char)((k << 1) | (k >> 7));
    }
    float out[8];
    _mm256_storeu_ps(out, x);
    uint64_t s = 0;
    for (int i = 0; i < 8; ++i) { union { float f; uint32_t u; } v; v.f = out[i]; s ^= (uint64_t)v.u << ((i & 1) ? 32 : 0); }
    return s;
#else
    return nc_bench_kernel_avx512(loops);
#endif
}

NC_NOINLINE uint64_t nc_bench_kernel_avx512vnni(size_t loops) {
#if defined(__AVX512VNNI__) && defined(__AVX512F__)
    __m512i acc = _mm512_setzero_si512();
    __m512i a = _mm512_set1_epi8(3);
    __m512i b = _mm512_set1_epi8(7);
    for (size_t i = 0; i < loops; ++i) {
        acc = _mm512_dpbusd_epi32(acc, a, b);
        a = _mm512_xor_si512(a, acc);
    }
    uint64_t out[8];
    _mm512_storeu_si512((__m512i*)out, acc);
    return out[0]^out[1]^out[2]^out[3]^out[4]^out[5]^out[6]^out[7];
#else
    return nc_bench_kernel_avx512(loops);
#endif
}
