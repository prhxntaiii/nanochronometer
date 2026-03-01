/* =============================================================================
 * demo.c  –  NanoChrono usage examples
 *
 * Shows how to use both the high-level context API and the raw ASM functions.
 * Build:  cl /O2 /DNANOCHRONO_DLL demo.c /link nanochrono_avx.lib
 * ============================================================================= */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <intrin.h>

#define NANOCHRONO_DLL
#include "nanochrono.h"

/* ─────────────────────────────────────────────────────────────────────────────
 * Helper: prints a separator
 * ───────────────────────────────────────────────────────────────────────────── */
static void sep(const char *title) {
    printf("\n══════════════════════════════════════════════\n");
    printf("  %s\n", title);
    printf("══════════════════════════════════════════════\n");
}

/* ─────────────────────────────────────────────────────────────────────────────
 * Dummy workload
 * ───────────────────────────────────────────────────────────────────────────── */
static volatile uint64_t sink;
static void do_work(int n) {
    uint64_t acc = 0;
    for (int i = 0; i < n; i++) acc += (uint64_t)i * 6364136223846793005ULL;
    sink = acc;
}

int main(void) {
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetThreadAffinityMask(GetCurrentThread(), 1ULL << 2);

    /* ── Detect backend ──────────────────────────────────────────────────── */
    sep("System info");

    int avx_ok = nc_is_avx_available();
    printf("  AVX + OS XSAVE: %s\n", avx_ok ? "YES" : "NO");

    char brand[49] = {0};
    nc_tsc_cpu_brand(brand);
    printf("  CPU brand     : %s\n", brand);
    printf("  CPUID max leaf: 0x%08X\n", nc_tsc_max_leaf());
    printf("  Invariant TSC : %s\n", nc_tsc_invariant() ? "YES" : "NO");

    /* ── Context API ─────────────────────────────────────────────────────── */
    sep("High-level context API");

    nc_ctx_t *ctx = nc_create();   /* auto-selects best backend */
    if (!ctx) { fprintf(stderr, "nc_create failed\n"); return 1; }

    printf("  Backend : %s\n",
           nc_backend(ctx) == NC_BACKEND_AVX ? "AVX (LFENCE)" : "LEGACY (CPUID)");
    printf("  TSC freq: %.3f MHz\n", (double)nc_tsc_hz(ctx) / 1e6);

    uint64_t overhead = nc_measure_overhead_cycles(ctx);
    printf("  Overhead: %llu cycles\n", (unsigned long long)overhead);

    /* measure a workload */
    nc_start(ctx);
    do_work(1000000);
    uint64_t stop = nc_stop(ctx);
    uint64_t delta = nc_tsc_delta(ctx->start_cycles, stop);  /* raw delta */
    /* NOTE: nc_ctx members are opaque – use nc_elapsed_* instead */
    uint64_t ns = nc_elapsed_ns(ctx);
    uint64_t us = nc_elapsed_us(ctx);

    printf("  Work (1M iter): %llu ns  /  %llu µs\n",
           (unsigned long long)ns, (unsigned long long)us);

    /* Formatted elapsed */
    char buf[32];
    nc_format_elapsed(ctx, buf, sizeof(buf));
    printf("  Formatted     : %s\n", buf);

    /* Drift */
    printf("  Drift         : %+.3f ppm\n", nc_drift_ppm(ctx));

    /* ── Precision sleep ─────────────────────────────────────────────────── */
    sep("Precision spin-sleep");

    nc_start(ctx);
    nc_sleep_us(ctx, 100);   /* sleep ~100 µs */
    uint64_t slept_ns = nc_elapsed_ns(ctx);
    printf("  Requested 100 µs, actual: %llu ns  (err: %+lld ns)\n",
           (unsigned long long)slept_ns,
           (long long)slept_ns - 100000LL);

    /* ── Low-level ASM (raw) ─────────────────────────────────────────────── */
    sep("Raw ASM – legacy backend");

    uint64_t t0, t1;
    t0 = nc_tsc_start();
    do_work(100000);
    t1 = nc_tsc_end();
    printf("  nc_tsc_start/end  : %llu cycles\n",
           (unsigned long long)(t1 - t0));

    t0 = nc_tsc_lfence_start();
    do_work(100000);
    t1 = nc_tsc_lfence_end();
    printf("  nc_tsc_lfence     : %llu cycles\n",
           (unsigned long long)(t1 - t0));

    printf("  Core / socket ID  : 0x%08X\n", nc_tsc_aux());

    nc_rdtscp_pair_t pair = {0};
    nc_tsc_rdtscp_pair(&pair);
    printf("  RDTSCP pair TSC   : 0x%016llX  aux=%u\n",
           (unsigned long long)pair.tsc, pair.aux);

    /* ── Low-level ASM (AVX) ──────────────────────────────────────────────── */
    if (avx_ok) {
        sep("Raw ASM – AVX backend");

        nc_avx_warmup();  /* clear pipeline before first measurement */

        t0 = nc_avx_tsc_start();
        do_work(100000);
        t1 = nc_avx_tsc_end();
        printf("  nc_avx_tsc_start/end : %llu cycles\n",
               (unsigned long long)(t1 - t0));

        t0 = nc_avx_tsc_full_start();   /* flushes YMM state */
        do_work(100000);
        t1 = nc_avx_tsc_full_end();
        printf("  nc_avx_tsc_full      : %llu cycles\n",
               (unsigned long long)(t1 - t0));

        printf("  AVX overhead         : %llu cycles\n",
               (unsigned long long)nc_avx_tsc_overhead());

        nc_avx_vzeroall();  /* clean YMM state for subsequent SSE code */
        printf("  VZEROALL done (YMM state cleared)\n");
    }

    /* ── Elapsed since first nc_start ────────────────────────────────────── */
    sep("Elapsed helpers");

    nc_reset(ctx);
    Sleep(10);  /* OS sleep ~10ms */
    printf("  ~10ms OS sleep → elapsed: %.3f ms\n",
           (double)nc_elapsed_ms(ctx));

    /* Conversion */
    uint64_t test_cycles = nc_tsc_hz(ctx);  /* exactly 1 second worth */
    printf("  1 second in cycles: %llu\n",
           (unsigned long long)test_cycles);
    printf("  cycles_to_ns(1s)  : %llu ns  (should be ~1e9)\n",
           (unsigned long long)nc_cycles_to_ns(ctx, test_cycles));

    nc_destroy(ctx);

    sep("Done");
    return 0;
}
