#ifndef _WIN32
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif
#include "nanochrono.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if defined(__x86_64__) || defined(_M_X64)
extern uint64_t nc_tsc_start(void);
extern uint64_t nc_tsc_end(void);
extern uint64_t nc_tsc_raw(void);
extern uint64_t nc_tsc_overhead(void);
extern uint64_t nc_avx_tsc_start(void);
extern uint64_t nc_avx_tsc_end(void);
extern uint64_t nc_avx_tsc_overhead(void);
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
extern uint64_t nc_arm64_scalar_counter(void);
extern uint64_t nc_arm64_scalar_overhead(void);
extern uint64_t nc_arm64_neon_counter(void);
extern uint64_t nc_arm64_neon_overhead(void);
extern uint64_t nc_arm64_sve_counter(void);
extern uint64_t nc_arm64_sve_overhead(void);
extern uint64_t nc_arm64_sve2_counter(void);
extern uint64_t nc_arm64_sve2_overhead(void);
extern uint64_t nc_arm64_sme_counter(void);
extern uint64_t nc_arm64_sme_overhead(void);
#endif
#ifndef NC_API
#if defined(_WIN32) && defined(NANOCHRONO_EXPORTS)
#define NC_API __declspec(dllexport)
#else
#define NC_API
#endif
#endif
struct nc_ctx { nc_backend_t backend; uint64_t hz; uint64_t start; uint64_t overhead; };
static uint64_t wall_ns(void){
#if defined(_WIN32)
    LARGE_INTEGER q,f; QueryPerformanceCounter(&q); QueryPerformanceFrequency(&f); return (uint64_t)((__int128)q.QuadPart*1000000000ull/f.QuadPart);
#else
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (uint64_t)ts.tv_sec*1000000000ull+(uint64_t)ts.tv_nsec;
#endif
}
static uint64_t counter_now(nc_backend_t b){
#if defined(__x86_64__) || defined(_M_X64)
    if(b==NC_BACKEND_AVX) return nc_avx_tsc_start();
    return nc_tsc_start();
#elif defined(__aarch64__) || defined(_M_ARM64)
    (void)b; return nc_arm64_scalar_counter();
#else
    (void)b; return wall_ns();
#endif
}
static uint64_t counter_overhead(nc_backend_t b){
#if defined(__x86_64__) || defined(_M_X64)
    if(b==NC_BACKEND_AVX) return nc_avx_tsc_overhead();
    return nc_tsc_overhead();
#elif defined(__aarch64__) || defined(_M_ARM64)
    (void)b; return nc_arm64_scalar_overhead();
#else
    (void)b; return 0;
#endif
}
NC_API nc_ctx_t *nc_create(void){ return nc_create_backend(nc_select_best_backend()); }
NC_API nc_ctx_t *nc_create_backend(nc_backend_t b){ nc_ctx_t *c=(nc_ctx_t*)calloc(1,sizeof(*c)); if(!c) return NULL; c->backend=b; c->hz=1000000000ull; nc_calibrate(c,20); c->overhead=counter_overhead(b); return c; }
NC_API void nc_destroy(nc_ctx_t *c){ free(c); }
NC_API int nc_calibrate(nc_ctx_t *c, uint32_t ms){ if(!c) return 0; if(ms<1) ms=1; uint64_t w0=wall_ns(), t0=counter_now(c->backend), w1=w0; while(w1-w0 < (uint64_t)ms*1000000ull) w1=wall_ns(); uint64_t t1=counter_now(c->backend); uint64_t dw=w1-w0, dt=t1-t0; c->hz = dw? (uint64_t)((__int128)dt*1000000000ull/dw) : 1000000000ull; if(!c->hz) c->hz=1000000000ull; return 1; }
NC_API void nc_reset(nc_ctx_t *c){ if(c) c->start=counter_now(c->backend); }
NC_API uint64_t nc_start(nc_ctx_t *c){ if(!c) return 0; c->start=counter_now(c->backend); return c->start; }
NC_API uint64_t nc_stop(nc_ctx_t *c){ return nc_elapsed_cycles(c); }
NC_API uint64_t nc_now_cycles(nc_ctx_t *c){ return counter_now(c?c->backend:NC_BACKEND_LEGACY); }
NC_API uint64_t nc_cycles_to_ns(nc_ctx_t *c,uint64_t cyc){ uint64_t hz=(c&&c->hz)?c->hz:1000000000ull; return (uint64_t)((__int128)cyc*1000000000ull/hz); }
NC_API uint64_t nc_cycles_to_us(nc_ctx_t *c,uint64_t cyc){ return nc_cycles_to_ns(c,cyc)/1000ull; }
NC_API uint64_t nc_cycles_to_ms(nc_ctx_t *c,uint64_t cyc){ return nc_cycles_to_ns(c,cyc)/1000000ull; }
NC_API double nc_cycles_to_sec(nc_ctx_t *c,uint64_t cyc){ uint64_t hz=(c&&c->hz)?c->hz:1000000000ull; return (double)cyc/(double)hz; }
NC_API uint64_t nc_ns_to_cycles(nc_ctx_t *c,uint64_t ns){ uint64_t hz=(c&&c->hz)?c->hz:1000000000ull; return (uint64_t)((__int128)ns*hz/1000000000ull); }
NC_API uint64_t nc_elapsed_cycles(nc_ctx_t *c){ if(!c) return 0; uint64_t n=counter_now(c->backend); return n>=c->start?n-c->start:0; }
NC_API uint64_t nc_elapsed_ns(nc_ctx_t *c){ return nc_cycles_to_ns(c,nc_elapsed_cycles(c)); }
NC_API uint64_t nc_elapsed_us(nc_ctx_t *c){ return nc_elapsed_ns(c)/1000ull; }
NC_API uint64_t nc_elapsed_ms(nc_ctx_t *c){ return nc_elapsed_ns(c)/1000000ull; }
NC_API double nc_elapsed_sec(nc_ctx_t *c){ return nc_cycles_to_sec(c,nc_elapsed_cycles(c)); }
NC_API void nc_sleep_ns(nc_ctx_t *c,uint64_t ns){ uint64_t end=wall_ns()+ns; (void)c; while(wall_ns()<end){} }
NC_API void nc_sleep_us(nc_ctx_t *c,uint64_t us){ nc_sleep_ns(c,us*1000ull); }
NC_API uint64_t nc_tsc_hz(nc_ctx_t *c){ return c?c->hz:1000000000ull; }
NC_API double nc_drift_ppm(nc_ctx_t *c){ (void)c; return 0.0; }
NC_API nc_backend_t nc_backend(nc_ctx_t *c){ return c?c->backend:NC_BACKEND_LEGACY; }
NC_API const char *nc_backend_name(nc_backend_t b){ switch(b){case NC_BACKEND_MMX:return "x64-mmx"; case NC_BACKEND_SSE:return "x64-sse"; case NC_BACKEND_SSE2:return "x64-sse2"; case NC_BACKEND_SSE3:return "x64-sse3"; case NC_BACKEND_SSSE3:return "x64-ssse3"; case NC_BACKEND_SSE41:return "x64-sse4.1"; case NC_BACKEND_SSE42:return "x64-sse4.2"; case NC_BACKEND_F16C:return "x64-f16c"; case NC_BACKEND_FMA:return "x64-fma"; case NC_BACKEND_AVX:return "x64-avx"; case NC_BACKEND_AVX2:return "x64-avx2"; case NC_BACKEND_AVX_VNNI:return "x64-avx-vnni"; case NC_BACKEND_AVX512:return "x64-avx512"; case NC_BACKEND_AVX512VNNI:return "x64-avx512-vnni"; default:return "scalar";} }
#if defined(__x86_64__) || defined(_M_X64)
static void nc_cpuid_(uint32_t leaf,uint32_t sub,uint32_t r[4]){
#if defined(_MSC_VER)
    int v[4]; __cpuidex(v,(int)leaf,(int)sub); r[0]=(uint32_t)v[0]; r[1]=(uint32_t)v[1]; r[2]=(uint32_t)v[2]; r[3]=(uint32_t)v[3];
#elif defined(__GNUC__) || defined(__clang__)
    uint32_t a,b,c,d; __asm__ __volatile__("cpuid":"=a"(a),"=b"(b),"=c"(c),"=d"(d):"a"(leaf),"c"(sub)); r[0]=a; r[1]=b; r[2]=c; r[3]=d;
#else
    r[0]=r[1]=r[2]=r[3]=0;
#endif
}
static uint64_t nc_xgetbv0_(void){
#if defined(_MSC_VER)
    return _xgetbv(0);
#elif defined(__GNUC__) || defined(__clang__)
    uint32_t a,d; __asm__ __volatile__("xgetbv":"=a"(a),"=d"(d):"c"(0)); return ((uint64_t)d<<32)|a;
#else
    return 0;
#endif
}
static void nc_detect_x64_(nc_cpu_features_t *o){ memset(o,0,sizeof(*o)); uint32_t r[4]={0}; nc_cpuid_(0,0,r); uint32_t max=r[0]; if(max<1) return; nc_cpuid_(1,0,r); uint32_t ecx=r[2], edx=r[3]; int osxsave=(ecx>>27)&1; uint64_t xcr0=osxsave?nc_xgetbv0_():0; int ymm=((xcr0&6)==6); int zmm=((xcr0&0xe6)==0xe6); o->mmx=(edx>>23)&1; o->sse=(edx>>25)&1; o->sse2=(edx>>26)&1; o->sse3=ecx&1; o->pclmulqdq=(ecx>>1)&1; o->ssse3=(ecx>>9)&1; o->fma=((ecx>>12)&1)&&ymm; o->sse41=(ecx>>19)&1; o->sse42=(ecx>>20)&1; o->aesni=(ecx>>25)&1; o->f16c=((ecx>>29)&1)&&ymm; o->avx=((ecx>>28)&1)&&ymm; if(max>=7){ nc_cpuid_(7,0,r); uint32_t submax=r[0], ebx=r[1], ecx7=r[2]; o->avx2=((ebx>>5)&1)&&ymm; o->shani=(ebx>>29)&1; o->avx512f=((ebx>>16)&1)&&zmm; o->avx512bw=((ebx>>30)&1)&&zmm; o->avx512vl=((ebx>>31)&1)&&zmm; o->avx512vnni=((ecx7>>11)&1)&&zmm; o->vaes=((ecx7>>9)&1)&&ymm; if(submax>=1){ nc_cpuid_(7,1,r); uint32_t eax71=r[0]; o->avx_vnni=((eax71>>4)&1)&&ymm&&o->avx2; } } }
#else
static void nc_detect_x64_(nc_cpu_features_t *o){ memset(o,0,sizeof(*o)); }
#endif
NC_API void nc_get_cpu_features(nc_cpu_features_t *o){ if(o) nc_detect_x64_(o); }
#define NC_FEAT_GETTER(field) NC_API int nc_cpu_has_##field(void){ nc_cpu_features_t f; nc_get_cpu_features(&f); return f.field; }
NC_FEAT_GETTER(mmx) NC_FEAT_GETTER(sse) NC_FEAT_GETTER(sse2) NC_FEAT_GETTER(sse3) NC_FEAT_GETTER(ssse3) NC_FEAT_GETTER(sse41) NC_FEAT_GETTER(sse42) NC_FEAT_GETTER(aesni) NC_FEAT_GETTER(shani) NC_FEAT_GETTER(vaes) NC_FEAT_GETTER(pclmulqdq) NC_FEAT_GETTER(f16c) NC_FEAT_GETTER(fma) NC_FEAT_GETTER(avx2) NC_FEAT_GETTER(avx_vnni) NC_FEAT_GETTER(avx512f) NC_FEAT_GETTER(avx512bw) NC_FEAT_GETTER(avx512vl) NC_FEAT_GETTER(avx512vnni)
NC_API int nc_is_avx_available(void){ return nc_cpu_has_avx2() || nc_cpu_has_fma() || nc_cpu_has_f16c(); }
NC_API int nc_backend_is_available(nc_backend_t b){ nc_cpu_features_t f; nc_get_cpu_features(&f); switch(b){ case NC_BACKEND_MMX:return f.mmx; case NC_BACKEND_SSE:return f.sse; case NC_BACKEND_SSE2:return f.sse2; case NC_BACKEND_SSE3:return f.sse3; case NC_BACKEND_SSSE3:return f.ssse3; case NC_BACKEND_SSE41:return f.sse41; case NC_BACKEND_SSE42:return f.sse42; case NC_BACKEND_F16C:return f.f16c; case NC_BACKEND_FMA:return f.fma; case NC_BACKEND_AVX:return f.avx; case NC_BACKEND_AVX2:return f.avx2; case NC_BACKEND_AVX_VNNI:return f.avx_vnni; case NC_BACKEND_AVX512:return f.avx512f; case NC_BACKEND_AVX512VNNI:return f.avx512vnni; default:return 1; } }
NC_API nc_backend_t nc_select_best_backend(void){ nc_cpu_features_t f; nc_get_cpu_features(&f); if(f.avx512vnni) return NC_BACKEND_AVX512VNNI; if(f.avx512f) return NC_BACKEND_AVX512; if(f.avx_vnni) return NC_BACKEND_AVX_VNNI; if(f.avx2) return NC_BACKEND_AVX2; if(f.fma) return NC_BACKEND_FMA; if(f.f16c) return NC_BACKEND_F16C; if(f.sse42) return NC_BACKEND_SSE42; if(f.sse41) return NC_BACKEND_SSE41; if(f.ssse3) return NC_BACKEND_SSSE3; if(f.sse3) return NC_BACKEND_SSE3; if(f.sse2) return NC_BACKEND_SSE2; if(f.sse) return NC_BACKEND_SSE; if(f.mmx) return NC_BACKEND_MMX; return NC_BACKEND_LEGACY; }

NC_API uint64_t nc_measure_overhead_cycles(nc_ctx_t *c){ return c?c->overhead:counter_overhead(NC_BACKEND_LEGACY); }
NC_API uint64_t nc_measure_call_overhead_cycles(nc_ctx_t *c, void (*fn)(void *), void *arg, uint32_t iters){ if(!fn||!iters) return 0; uint64_t best=~0ull; for(uint32_t i=0;i<iters;i++){ uint64_t a=counter_now(c?c->backend:NC_BACKEND_LEGACY); fn(arg); uint64_t b=counter_now(c?c->backend:NC_BACKEND_LEGACY); uint64_t d=b-a; if(d<best) best=d; } return best; }
NC_API uint64_t nc_measure_ffi_overhead_cycles(nc_ctx_t *c, uint32_t iters){ if(!iters) iters=10000; uint64_t best=~0ull; for(uint32_t i=0;i<iters;i++){ uint64_t a=counter_now(c?c->backend:NC_BACKEND_LEGACY); uint64_t b=counter_now(c?c->backend:NC_BACKEND_LEGACY); uint64_t d=b-a; if(d<best) best=d; } return best; }
NC_API void nc_format_ns(uint64_t ns,char *buf,size_t cap){ if(!buf||!cap) return; uint64_t h=ns/3600000000000ull; ns%=3600000000000ull; uint64_t m=ns/60000000000ull; ns%=60000000000ull; uint64_t s=ns/1000000000ull; ns%=1000000000ull; snprintf(buf,cap,"%02llu:%02llu:%02llu:%03llu:%03llu:%03llu",(unsigned long long)h,(unsigned long long)m,(unsigned long long)s,(unsigned long long)(ns/1000000ull),(unsigned long long)((ns/1000ull)%1000ull),(unsigned long long)(ns%1000ull)); }
NC_API void nc_format_elapsed(nc_ctx_t *c,char *buf,size_t cap){ nc_format_ns(nc_elapsed_ns(c),buf,cap); }

/* -------------------------------------------------------------------------
 * NanoChronometer Toolkit implementation
 * ---------------------------------------------------------------------- */
#include <math.h>
#if !defined(_WIN32)
#include <sched.h>
#include <unistd.h>
#endif

static int nc_u64_cmp_(const void *pa, const void *pb){
    uint64_t a=*(const uint64_t*)pa,b=*(const uint64_t*)pb;
    return (a>b)-(a<b);
}

NC_API const char *nc_toolkit_version(void){ return "NanoChronometer Toolkit 0.4.0"; }
NC_API uint64_t nc_wall_time_ns(void){ return wall_ns(); }
NC_API uint64_t nc_monotonic_time_ns(void){ return wall_ns(); }
NC_API uint64_t nc_process_time_ns(void){
#if defined(_WIN32)
    FILETIME c,e,k,u; if(GetProcessTimes(GetCurrentProcess(),&c,&e,&k,&u)){ ULARGE_INTEGER ku,uu; ku.LowPart=k.dwLowDateTime; ku.HighPart=k.dwHighDateTime; uu.LowPart=u.dwLowDateTime; uu.HighPart=u.dwHighDateTime; return (ku.QuadPart+uu.QuadPart)*100ull; }
    return wall_ns();
#elif defined(CLOCK_PROCESS_CPUTIME_ID)
    struct timespec ts; clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ts); return (uint64_t)ts.tv_sec*1000000000ull+(uint64_t)ts.tv_nsec;
#else
    return wall_ns();
#endif
}
NC_API uint64_t nc_thread_time_ns(void){
#if defined(_WIN32)
    FILETIME c,e,k,u; if(GetThreadTimes(GetCurrentThread(),&c,&e,&k,&u)){ ULARGE_INTEGER ku,uu; ku.LowPart=k.dwLowDateTime; ku.HighPart=k.dwHighDateTime; uu.LowPart=u.dwLowDateTime; uu.HighPart=u.dwHighDateTime; return (ku.QuadPart+uu.QuadPart)*100ull; }
    return wall_ns();
#elif defined(CLOCK_THREAD_CPUTIME_ID)
    struct timespec ts; clock_gettime(CLOCK_THREAD_CPUTIME_ID,&ts); return (uint64_t)ts.tv_sec*1000000000ull+(uint64_t)ts.tv_nsec;
#else
    return wall_ns();
#endif
}

NC_API void nc_compiler_barrier(void){
#if defined(_MSC_VER)
    _ReadWriteBarrier();
#else
    __asm__ __volatile__("" ::: "memory");
#endif
}
NC_API void nc_memory_barrier(void){
#if defined(_MSC_VER)
    _ReadWriteBarrier();
#elif defined(__GNUC__) || defined(__clang__)
    __sync_synchronize();
#endif
}
NC_API void nc_cpu_relax(void){
#if defined(__x86_64__) || defined(_M_X64)
    nc_asm_pause();
#elif defined(__aarch64__) || defined(_M_ARM64)
    nc_arm64_yield();
#else
#endif
}
NC_API uint64_t nc_empty_call(void){ return 0; }
NC_API uint64_t nc_empty_call_u64(uint64_t x){ return x; }

NC_API uint64_t nc_measure_callback_min_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters){
    return nc_measure_call_overhead_cycles(ctx, fn, arg, iters);
}
NC_API uint64_t nc_measure_callback_avg_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters){
    if(!fn || !iters) return 0;
    uint64_t sum=0; nc_backend_t b=ctx?ctx->backend:NC_BACKEND_LEGACY;
    for(uint32_t i=0;i<iters;i++){ uint64_t a=counter_now(b); fn(arg); uint64_t e=counter_now(b); sum += e-a; }
    return sum/iters;
}
NC_API uint64_t nc_measure_callback_ns(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint32_t iters){
    return nc_cycles_to_ns(ctx, nc_measure_callback_avg_cycles(ctx,fn,arg,iters));
}
NC_API uint64_t nc_measure_dll_boundary_cycles(nc_ctx_t *ctx, uint32_t iters){ return nc_measure_ffi_overhead_cycles(ctx,iters); }
NC_API uint64_t nc_measure_python_wheel_boundary_cycles(nc_ctx_t *ctx, uint32_t iters){ return nc_measure_ffi_overhead_cycles(ctx,iters); }

NC_API int nc_collect_samples_cycles(nc_ctx_t *ctx, nc_void_callback_t fn, void *arg, uint64_t *samples, uint32_t count){
    if(!fn || !samples || !count) return 0;
    nc_backend_t b=ctx?ctx->backend:NC_BACKEND_LEGACY;
    for(uint32_t i=0;i<count;i++){ uint64_t a=counter_now(b); fn(arg); uint64_t e=counter_now(b); samples[i]=e-a; }
    return 1;
}
NC_API uint64_t nc_percentile_cycles(uint64_t *samples, uint32_t count, double p){
    if(!samples || !count) {
        return 0;
    }
    if(p < 0.0) {
        p = 0.0;
    }
    if(p > 100.0) {
        p = 100.0;
    }
    qsort(samples, count, sizeof(uint64_t), nc_u64_cmp_);
    uint32_t idx = (uint32_t)((p/100.0) * (double)(count - 1));
    return samples[idx];
}
NC_API uint64_t nc_median_cycles(uint64_t *samples, uint32_t count){ return nc_percentile_cycles(samples,count,50.0); }
NC_API int nc_analyze_samples(const uint64_t *samples, uint32_t count, nc_sample_stats_t *o){
    if(!samples || !count || !o) {
        return 0;
    }
    memset(o, 0, sizeof(*o));
    uint64_t *tmp = (uint64_t*)malloc((size_t)count * sizeof(uint64_t));
    if(!tmp) {
        return 0;
    }
    uint64_t sum = 0, minv = ~0ull, maxv = 0;
    for(uint32_t i = 0; i < count; i++) {
        uint64_t v = samples[i];
        tmp[i] = v;
        sum += v;
        if(v < minv) minv = v;
        if(v > maxv) maxv = v;
    }
    double mean = (double)sum / (double)count;
    double var = 0.0;
    for(uint32_t i = 0; i < count; i++) {
        double d = (double)samples[i] - mean;
        var += d * d;
    }
    var /= (double)count;
    qsort(tmp, count, sizeof(uint64_t), nc_u64_cmp_);
    o->count = count;
    o->min_cycles = minv;
    o->max_cycles = maxv;
    o->mean_cycles = (uint64_t)mean;
    o->median_cycles = tmp[(count - 1) / 2];
    o->p90_cycles = tmp[(uint32_t)(0.90 * (double)(count - 1))];
    o->p99_cycles = tmp[(uint32_t)(0.99 * (double)(count - 1))];
    o->variance_cycles = var;
    o->stdev_cycles = sqrt(var);
    free(tmp);
    return 1;
}
NC_API double nc_welch_t_score(const uint64_t *a, uint32_t na, const uint64_t *b, uint32_t nb){
    if(!a || !b || na < 2 || nb < 2) {
        return 0.0;
    }
    double ma = 0.0, mb = 0.0, va = 0.0, vb = 0.0;
    for(uint32_t i = 0; i < na; i++) {
        ma += (double)a[i];
    }
    ma /= (double)na;
    for(uint32_t i = 0; i < nb; i++) {
        mb += (double)b[i];
    }
    mb /= (double)nb;
    for(uint32_t i = 0; i < na; i++) {
        double d = (double)a[i] - ma;
        va += d * d;
    }
    va /= (double)(na - 1);
    for(uint32_t i = 0; i < nb; i++) {
        double d = (double)b[i] - mb;
        vb += d * d;
    }
    vb /= (double)(nb - 1);
    double den = sqrt(va / (double)na + vb / (double)nb);
    return den ? ((ma - mb) / den) : 0.0;
}

NC_API uint64_t nc_time_crypto_call_cycles(nc_ctx_t *ctx, nc_crypto_callback_t fn, void *out, size_t *out_len, const void *in, size_t in_len, void *arg, int *status_out){
    if(!fn) {
        return 0;
    }
    nc_backend_t b = ctx ? ctx->backend : NC_BACKEND_LEGACY;
    uint64_t a = counter_now(b);
    int st = fn(out, out_len, in, in_len, arg);
    uint64_t e = counter_now(b);
    if(status_out) {
        *status_out = st;
    }
    return e - a;
}
NC_API int nc_time_crypto_many(nc_ctx_t *ctx, nc_crypto_callback_t fn, void *out, size_t *out_len, const void *in, size_t in_len, void *arg, uint64_t *samples, uint32_t count){
    if(!fn || !samples || !count) {
        return 0;
    }
    for(uint32_t i = 0; i < count; i++) {
        samples[i] = nc_time_crypto_call_cycles(ctx, fn, out, out_len, in, in_len, arg, NULL);
    }
    return 1;
}
NC_API int nc_constant_time_eq(const void *a, const void *b, size_t n){
    const uint8_t *pa=(const uint8_t*)a,*pb=(const uint8_t*)b; uint8_t diff=0; if(!a||!b) return 0; for(size_t i=0;i<n;i++) diff|=(uint8_t)(pa[i]^pb[i]); return diff==0;
}
NC_API uint64_t nc_time_memcmp_cycles(nc_ctx_t *ctx, const void *a, const void *b, size_t n, int *cmp_out){
    nc_backend_t be=ctx?ctx->backend:NC_BACKEND_LEGACY; uint64_t s=counter_now(be); int r=memcmp(a,b,n); uint64_t e=counter_now(be); if(cmp_out)*cmp_out=r; return e-s;
}
NC_API uint64_t nc_time_constant_time_eq_cycles(nc_ctx_t *ctx, const void *a, const void *b, size_t n, int *eq_out){
    nc_backend_t be=ctx?ctx->backend:NC_BACKEND_LEGACY; uint64_t s=counter_now(be); int r=nc_constant_time_eq(a,b,n); uint64_t e=counter_now(be); if(eq_out)*eq_out=r; return e-s;
}
NC_API uint64_t nc_time_memory_load_cycles(nc_ctx_t *ctx, const void *ptr, size_t bytes, uint64_t *checksum_out){
    volatile const uint8_t *p=(volatile const uint8_t*)ptr; uint64_t x=0; nc_backend_t be=ctx?ctx->backend:NC_BACKEND_LEGACY; uint64_t s=counter_now(be); for(size_t i=0;i<bytes;i++) x^=p[i]; uint64_t e=counter_now(be); if(checksum_out)*checksum_out=x; return e-s;
}
NC_API uint64_t nc_time_memory_store_cycles(nc_ctx_t *ctx, void *ptr, size_t bytes, uint8_t value){
    volatile uint8_t *p=(volatile uint8_t*)ptr; nc_backend_t be=ctx?ctx->backend:NC_BACKEND_LEGACY; uint64_t s=counter_now(be); for(size_t i=0;i<bytes;i++) p[i]=value; uint64_t e=counter_now(be); return e-s;
}
NC_API uint64_t nc_time_flush_reload_cycles(nc_ctx_t *ctx, const void *ptr){
#if defined(__x86_64__) || defined(_M_X64)
    nc_asm_clflush(ptr); nc_asm_mfence();
#endif
    uint64_t sink=0; return nc_time_memory_load_cycles(ctx,ptr,1,&sink);
}
NC_API uint64_t nc_time_prefetch_reload_cycles(nc_ctx_t *ctx, const void *ptr){
#if defined(__x86_64__) || defined(_M_X64)
    nc_asm_prefetcht0(ptr); nc_asm_lfence();
#endif
    uint64_t sink=0; return nc_time_memory_load_cycles(ctx,ptr,1,&sink);
}
