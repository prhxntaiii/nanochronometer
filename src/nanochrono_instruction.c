#include "nanochrono.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL)
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#endif
#if defined(NC_USE_LIBSODIUM)
#include <sodium.h>
#endif

static uint64_t nc_tick_(nc_ctx_t *ctx){ return ctx ? nc_now_cycles(ctx) : nc_wall_time_ns(); }
static uint64_t nc_to_ns_(nc_ctx_t *ctx, uint64_t c){ return ctx ? nc_cycles_to_ns(ctx,c) : c; }
static void nc_fill_result_(nc_ctx_t *ctx, nc_instruction_result_t *o, int st, nc_instruction_family_t fam, uint64_t cyc, uint64_t blocks, uint64_t sum){ if(o){ o->status=st; o->family=(uint32_t)fam; o->backend=(uint32_t)nc_backend(ctx); o->cycles=cyc; o->ns=nc_to_ns_(ctx,cyc); o->blocks=blocks; o->checksum=sum; } }

const char *nc_instruction_family_name(nc_instruction_family_t f){ switch(f){ case NC_INSTR_SCALAR:return "scalar"; case NC_INSTR_AES:return "aes"; case NC_INSTR_SHA:return "sha"; case NC_INSTR_PCLMULQDQ:return "pclmulqdq/pmull"; case NC_INSTR_CRC32:return "crc32"; case NC_INSTR_SSE2:return "sse2"; case NC_INSTR_AVX2:return "avx2"; case NC_INSTR_AVX512:return "avx512"; case NC_INSTR_NEON:return "neon"; case NC_INSTR_SVE:return "sve"; case NC_INSTR_SVE2:return "sve2"; case NC_INSTR_SME:return "sme"; case NC_INSTR_SME2:return "sme2"; default:return "unknown"; } }

int nc_instruction_family_available(nc_instruction_family_t f){
#if defined(__x86_64__) || defined(_M_X64)
    switch(f){ case NC_INSTR_SCALAR:return 1; case NC_INSTR_AES:return nc_cpu_has_aesni(); case NC_INSTR_SHA:return nc_cpu_has_shani(); case NC_INSTR_PCLMULQDQ:return nc_cpu_has_pclmulqdq(); case NC_INSTR_CRC32:return nc_cpu_has_sse42(); case NC_INSTR_SSE2:return nc_cpu_has_sse2(); case NC_INSTR_AVX2:return nc_cpu_has_avx2(); case NC_INSTR_AVX512:return nc_cpu_has_avx512f(); default:return 0; }
#elif defined(__aarch64__) || defined(_M_ARM64)
    switch(f){ case NC_INSTR_SCALAR: case NC_INSTR_NEON: case NC_INSTR_AES: case NC_INSTR_SHA: case NC_INSTR_PCLMULQDQ: return 1; case NC_INSTR_SVE: return nc_asm_simd_family_available(NC_ASM_SIMD_ARM64_SVE); case NC_INSTR_SVE2: return nc_asm_simd_family_available(NC_ASM_SIMD_ARM64_SVE2); case NC_INSTR_SME: return nc_asm_simd_family_available(NC_ASM_SIMD_ARM64_SME); case NC_INSTR_SME2: return nc_asm_simd_family_available(NC_ASM_SIMD_ARM64_SME2); default:return 0; }
#else
    return f==NC_INSTR_SCALAR;
#endif
}

uint64_t nc_measure_aesenc_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out){ if(!blocks) blocks=1024; if(!nc_instruction_family_available(NC_INSTR_AES)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,NC_INSTR_AES,0,blocks,0); return 0; } uint64_t s=nc_tick_(ctx);
#if defined(__x86_64__) || defined(_M_X64)
    uint64_t sum=nc_asm_aesenc_128_loop(blocks);
#elif defined(__aarch64__) || defined(_M_ARM64)
    uint64_t sum=nc_arm64_neon_aes_loop(blocks);
#else
    uint64_t sum=0; for(uint32_t i=0;i<blocks;i++) sum^=(uint64_t)i*0x9e3779b97f4a7c15ull;
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,NC_INSTR_AES,d,blocks,sum); return d; }

uint64_t nc_measure_sha256msg_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out){ if(!blocks) blocks=1024; if(!nc_instruction_family_available(NC_INSTR_SHA)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,NC_INSTR_SHA,0,blocks,0); return 0; } uint64_t s=nc_tick_(ctx);
#if defined(__x86_64__) || defined(_M_X64)
    uint64_t sum=nc_asm_sha256rnds2_loop(blocks);
#elif defined(__aarch64__) || defined(_M_ARM64)
    uint64_t sum=nc_arm64_neon_sha256_loop(blocks);
#else
    uint64_t sum=0; for(uint32_t i=0;i<blocks;i++) sum += i;
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,NC_INSTR_SHA,d,blocks,sum); return d; }

uint64_t nc_measure_pclmul_cycles(nc_ctx_t *ctx, uint32_t blocks, nc_instruction_result_t *out){ if(!blocks) blocks=1024; if(!nc_instruction_family_available(NC_INSTR_PCLMULQDQ)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,NC_INSTR_PCLMULQDQ,0,blocks,0); return 0; } uint64_t s=nc_tick_(ctx);
#if defined(__x86_64__) || defined(_M_X64)
    uint64_t sum=nc_asm_pclmul_loop(blocks);
#elif defined(__aarch64__) || defined(_M_ARM64)
    uint64_t sum=nc_arm64_neon_pmull_loop(blocks);
#else
    uint64_t sum=0; for(uint32_t i=0;i<blocks;i++) sum ^= (uint64_t)i << 32;
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,NC_INSTR_PCLMULQDQ,d,blocks,sum); return d; }

uint64_t nc_measure_vector_xor_cycles(nc_ctx_t *ctx, nc_instruction_family_t fam, const void *a, const void *b, void *out_buf, size_t bytes, nc_instruction_result_t *out){ if(!a||!b||!out_buf){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,fam,0,0,0); return 0; } if(!nc_instruction_family_available(fam)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,fam,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); uint64_t sum=0;
#if defined(__x86_64__) || defined(_M_X64)
    (void)fam; sum=nc_asm_vector_xor_loop(a,b,out_buf,bytes);
#elif defined(__aarch64__) || defined(_M_ARM64)
    if(fam==NC_INSTR_SVE || fam==NC_INSTR_SVE2) sum=nc_arm64_sve_vector_xor_loop(a,b,out_buf,bytes); else sum=nc_arm64_neon_vector_xor_loop(a,b,out_buf,bytes);
#else
    const uint8_t *pa=a,*pb=b; uint8_t *po=out_buf; for(size_t i=0;i<bytes;i++){ po[i]=pa[i]^pb[i]; sum^=po[i]; }
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,fam,d,bytes/16,sum); return d; }

uint64_t nc_measure_vector_add_cycles(nc_ctx_t *ctx, nc_instruction_family_t fam, const void *a, const void *b, void *out_buf, size_t bytes, nc_instruction_result_t *out){ if(!a||!b||!out_buf||bytes%8){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,fam,0,0,0); return 0; } if(!nc_instruction_family_available(fam)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,fam,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); uint64_t sum=0;
#if defined(__x86_64__) || defined(_M_X64)
    (void)fam; sum=nc_asm_vector_add_u64_loop(a,b,out_buf,bytes/8);
#else
    const uint64_t *pa=a,*pb=b; uint64_t *po=out_buf; for(size_t i=0;i<bytes/8;i++){ po[i]=pa[i]+pb[i]; sum^=po[i]; }
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,fam,d,bytes/8,sum); return d; }

uint64_t nc_measure_instruction_family_cycles(nc_ctx_t *ctx, nc_instruction_family_t family, uint32_t iterations, nc_instruction_result_t *out){ if(!iterations) iterations=1024; switch(family){ case NC_INSTR_AES: return nc_measure_aesenc_cycles(ctx,iterations,out); case NC_INSTR_SHA: return nc_measure_sha256msg_cycles(ctx,iterations,out); case NC_INSTR_PCLMULQDQ: return nc_measure_pclmul_cycles(ctx,iterations,out); case NC_INSTR_CRC32: { if(!nc_instruction_family_available(family)){ nc_fill_result_(ctx,out,NC_ERR_UNSUPPORTED,family,0,iterations,0); return 0; } uint8_t buf[4096]; memset(buf,0xa5,sizeof(buf)); uint64_t s=nc_tick_(ctx); uint64_t sum=nc_asm_crc32_loop(buf,sizeof(buf)); uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,family,d,sizeof(buf),sum); return d; } default: { uint64_t x=0,s=nc_tick_(ctx); for(uint32_t i=0;i<iterations;i++) x += nc_empty_call_u64(i); uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,family,d,iterations,x); return d; }} }

uint64_t nc_measure_cache_probe_cycles(nc_ctx_t *ctx, const void *ptr, nc_sidechannel_result_t *out){ if(!ptr){ if(out) memset(out,0,sizeof(*out)); return 0; } uint64_t c=nc_time_memory_load_cycles(ctx,ptr,1,0); uint64_t f=nc_time_flush_reload_cycles(ctx,ptr); uint64_t p=nc_time_prefetch_reload_cycles(ctx,ptr); if(out){ out->status=NC_OK; out->cached_cycles=c; out->flushed_cycles=f; out->prefetched_cycles=p; out->threshold_cycles=(c+f)/2; out->separation_score=(double)f-(double)c; } return f; }

uint64_t nc_measure_branch_mispredict_cycles(nc_ctx_t *ctx, const uint8_t *pattern, size_t count, nc_instruction_result_t *out){ if(!pattern||!count){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,NC_INSTR_SCALAR,0,0,0); return 0; } uint64_t s=nc_tick_(ctx), sum=nc_asm_branch_probe_loop(pattern,count), e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,NC_OK,NC_INSTR_SCALAR,d,count,sum); return d; }

int nc_crypto_backend_mask(void){
    int m=0;
#if defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL)
    m|=1;
#endif
#if defined(NC_USE_LIBSODIUM)
    m|=2;
#endif
    return m;
}
uint64_t nc_crypto_rand_cycles(nc_ctx_t *ctx, void *out_buf, size_t bytes, nc_instruction_result_t *out){ if(!out_buf){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,NC_INSTR_SCALAR,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); int st=NC_ERR_CRYPTO_BACKEND;
#if defined(NC_USE_LIBSODIUM)
    randombytes_buf(out_buf,bytes); st=NC_OK;
#elif defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL)
    st=(RAND_bytes((unsigned char*)out_buf,(int)bytes)==1)?NC_OK:NC_ERR_CRYPTO_BACKEND;
#else
    memset(out_buf,0,bytes);
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,st,NC_INSTR_SCALAR,d,bytes,0); return d; }
uint64_t nc_crypto_sha256_cycles(nc_ctx_t *ctx, const void *msg, size_t len, uint8_t out_digest[32], nc_instruction_result_t *out){ if(!msg||!out_digest){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,NC_INSTR_SHA,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); int st=NC_ERR_CRYPTO_BACKEND;
#if defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL)
    st=SHA256((const unsigned char*)msg,len,out_digest)?NC_OK:NC_ERR_CRYPTO_BACKEND;
#elif defined(NC_USE_LIBSODIUM)
    st=crypto_hash_sha256(out_digest,(const unsigned char*)msg,(unsigned long long)len)==0?NC_OK:NC_ERR_CRYPTO_BACKEND;
#else
    memset(out_digest,0,32);
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,st,NC_INSTR_SHA,d,len/64,0); return d; }
uint64_t nc_crypto_hmac_sha256_cycles(nc_ctx_t *ctx, const void *key, size_t key_len, const void *msg, size_t msg_len, uint8_t out_mac[32], nc_instruction_result_t *out){ if(!key||!msg||!out_mac){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,NC_INSTR_SHA,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); int st=NC_ERR_CRYPTO_BACKEND;
#if defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL)
    unsigned int l=0; st=HMAC(EVP_sha256(),key,(int)key_len,(const unsigned char*)msg,msg_len,out_mac,&l)?NC_OK:NC_ERR_CRYPTO_BACKEND;
#else
    (void)key_len; memset(out_mac,0,32);
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,st,NC_INSTR_SHA,d,msg_len/64,0); return d; }
uint64_t nc_crypto_aead_chacha20poly1305_cycles(nc_ctx_t *ctx, const uint8_t *key, const uint8_t *nonce, const uint8_t *aad, size_t aad_len, const uint8_t *msg, size_t msg_len, uint8_t *cipher, uint8_t tag[16], nc_instruction_result_t *out){ if(!key||!nonce||!msg||!cipher||!tag){ nc_fill_result_(ctx,out,NC_ERR_BAD_ARGUMENT,NC_INSTR_SCALAR,0,0,0); return 0; } uint64_t s=nc_tick_(ctx); int st=NC_ERR_CRYPTO_BACKEND;
#if defined(NC_USE_LIBSODIUM)
    unsigned long long clen=0; st=crypto_aead_chacha20poly1305_ietf_encrypt_detached(cipher,tag,&clen,msg,(unsigned long long)msg_len,aad,(unsigned long long)aad_len,NULL,nonce,key)==0?NC_OK:NC_ERR_CRYPTO_BACKEND;
#else
    (void)aad; (void)aad_len; memcpy(cipher,msg,msg_len); memset(tag,0,16);
#endif
    uint64_t e=nc_tick_(ctx), d=e-s; nc_fill_result_(ctx,out,st,NC_INSTR_SCALAR,d,msg_len/64,0); return d; }
