#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#  define _POSIX_C_SOURCE 200809L
#endif

#include "nanochrono.h"

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <time.h>
#endif

static volatile sig_atomic_t g_nc_stopwatch_stop = 0;

static void nc_cli_signal_handler(int sig) {
    (void)sig;
    g_nc_stopwatch_stop = 1;
}

static void nc_cli_sleep_ms(unsigned ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000ul);
    nanosleep(&ts, NULL);
#endif
}

static void nc_cli_usage(const char *exe) {
    printf("Usage:\n");
    printf("  %s                 run live stopwatch until Ctrl+C\n", exe);
    printf("  %s --stopwatch     same as no arguments\n", exe);
    printf("  %s --once          print one diagnostic timing sample and exit\n", exe);
    printf("  %s --catalog       list library microbench catalog entries\n", exe);
    printf("  %s --whitebox      run one safe white-box ASM microbench\n", exe);
    printf("  %s --sct-audit     run a local constant-time timing audit demo\n", exe);
    printf("  %s --asm-probe     run low-level local ASM timing probes\n", exe);
    printf("  %s --asm-simd      run low-level SIMD ASM timing probes with auto route gating\n", exe);
    printf("  %s --clock [--simple|--nano] [--local|--utc|--utc-offset MIN] [--ntp SERVER]  live precise clock app view\n", exe);
    printf("  %s --clock-once [--simple|--nano] [--local|--utc|--utc-offset MIN] [--ntp SERVER] one precise clock sample\n", exe);
    printf("  %s --calibrate-clock [--pin-cpu N] calibrate selected counter route and detect migration\n", exe);
    printf("  %s --stable-calibrate [--pin-cpu N] [--ms N] calibrate cycles_per_ns for raw RDTSC/RDTSCP/CNTVCT conversion\n", exe);
    printf("  %s --cycles-to-ns CYCLES --cycles-per-ns F convert calibrated raw cycles/ticks to ns\n", exe);
    printf("  %s --ntp SERVER   query an NTP server and report offset/delay/overhead\n", exe);
    printf("  %s --ns-clock      live nanosecond wall-clock plus raw CPU counters\n", exe);
    printf("  %s --ns-clock-once print one nanosecond clock snapshot\n", exe);
    printf("  %s --wrapper-overhead measure native and language-wrapper baselines\n", exe);
    printf("  %s --help          show this help\n", exe);
}


static void nc_cli_candidate_ct(uint8_t *out, size_t out_len, const uint8_t *in, size_t in_len, void *arg) {
    (void)arg;
    uint8_t acc = 0;
    for (size_t i = 0; i < in_len; ++i) acc |= (uint8_t)(in[i] ^ 0xA5u);
    if (out && out_len) memset(out, acc, out_len);
}



static nc_time_zone_mode_t nc_cli_tz_mode = NC_TIME_ZONE_LOCAL;
static int32_t nc_cli_tz_offset_min = 0;
static nc_clock_detail_mode_t nc_cli_detail = NC_CLOCK_DETAIL_SIMPLE;

static int nc_cli_parse_tz_offset(const char *s, int32_t *out) {
    if (!s || !out) return 0;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end && *end == '\0') { *out = (int32_t)v; return 1; }
    int sign = 1, hh = 0, mm = 0;
    if (*s == '-') { sign = -1; ++s; } else if (*s == '+') { ++s; }
    if (sscanf(s, "%d:%d", &hh, &mm) == 2) { *out = (int32_t)(sign * (hh * 60 + mm)); return 1; }
    return 0;
}

static void nc_cli_parse_clock_options(int argc, char **argv, const char **ntp_server, int *pin_cpu, uint32_t *cpu_index) {
    nc_cli_tz_mode = NC_TIME_ZONE_LOCAL;
    nc_cli_tz_offset_min = 0;
    nc_cli_detail = NC_CLOCK_DETAIL_SIMPLE;
    if (ntp_server) *ntp_server = NULL;
    if (pin_cpu) *pin_cpu = 0;
    if (cpu_index) *cpu_index = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--nano") == 0) nc_cli_detail = NC_CLOCK_DETAIL_NANO;
        else if (strcmp(argv[i], "--simple") == 0) nc_cli_detail = NC_CLOCK_DETAIL_SIMPLE;
        else if (strcmp(argv[i], "--utc") == 0) nc_cli_tz_mode = NC_TIME_ZONE_UTC;
        else if (strcmp(argv[i], "--local") == 0) nc_cli_tz_mode = NC_TIME_ZONE_LOCAL;
        else if (strcmp(argv[i], "--utc-offset") == 0 && i + 1 < argc) {
            nc_cli_tz_mode = NC_TIME_ZONE_CUSTOM_OFFSET;
            nc_cli_parse_tz_offset(argv[++i], &nc_cli_tz_offset_min);
        } else if (strcmp(argv[i], "--ntp") == 0 && i + 1 < argc) {
            if (ntp_server) *ntp_server = argv[++i]; else ++i;
        } else if (strcmp(argv[i], "--pin-cpu") == 0 && i + 1 < argc) {
            if (pin_cpu) *pin_cpu = 1;
            if (cpu_index) *cpu_index = (uint32_t)strtoul(argv[++i], NULL, 10); else ++i;
        }
    }
}

static void nc_cli_print_precision_clock(nc_ctx_t *ctx, const char *ntp_server) {
    nc_nanoclock_snapshot_t snap;
    char raw_local[96], display[96];
    memset(&snap, 0, sizeof(snap));
    if (!nc_nanoclock_snapshot(ctx, &snap)) {
        printf("clock: snapshot failed\n");
        return;
    }
    nc_format_unix_time_ns_ex(snap.unix_time_ns, NC_TIME_ZONE_LOCAL, 0, raw_local, sizeof(raw_local));
    nc_format_unix_time_ns_ex(snap.unix_time_ns, nc_cli_tz_mode, nc_cli_tz_offset_min, display, sizeof(display));
    printf("CLOCK view=%s mode=%s zone=%s\n", nc_clock_view_name(NC_CLOCK_VIEW_CLOCK), nc_clock_detail_mode_name(nc_cli_detail), nc_time_zone_mode_name(nc_cli_tz_mode));
    printf("raw_local_no_ntp=%s\n", raw_local);
    printf("display_time=%s\n", display);
    if (nc_cli_detail == NC_CLOCK_DETAIL_NANO) {
        printf("route=%s simd=%s cpu=%u raw=%llu selected_ns=%llu kernel_call_overhead=%llu api_call_overhead=%llu wrapper_ctypes_hint=%llu\n",
               nc_clock_route_name((nc_clock_route_t)snap.route),
               nc_asm_simd_family_name((nc_asm_simd_family_t)snap.best_simd_family),
               nc_clock_current_cpu(),
               (unsigned long long)snap.selected_raw_units,
               (unsigned long long)snap.selected_ns,
               (unsigned long long)nc_measure_kernel_timecall_overhead_cycles(ctx, 256),
               (unsigned long long)nc_measure_api_call_overhead_cycles(ctx, 256),
               (unsigned long long)snap.wrapper_hint_cycles);
#if defined(__x86_64__) || defined(_M_X64)
        printf("x64 rdtsc=%llu lfence_rdtsc=%llu mfence_lfence_rdtsc=%llu rdtscp_lfence=%llu aux=%u\n",
               (unsigned long long)snap.rdtsc_raw,
               (unsigned long long)snap.rdtsc_lfence,
               (unsigned long long)snap.rdtsc_mfence,
               (unsigned long long)snap.rdtscp_lfence,
               snap.rdtscp_aux);
#elif defined(__aarch64__) || defined(_M_ARM64)
        printf("arm64 cntfrq_el0=%llu cntvct_el0=%llu cntvct_isb=%llu pmccntr_el0=%llu pmccntr_status=%u\n",
               (unsigned long long)snap.arm64_cntfrq_el0,
               (unsigned long long)snap.arm64_cntvct_el0,
               (unsigned long long)snap.arm64_cntvct_isb,
               (unsigned long long)snap.arm64_pmccntr_el0,
               snap.arm64_pmccntr_status);
#endif
    }
    if (ntp_server && ntp_server[0]) {
        nc_ntp_sample_t ntp;
        if (nc_ntp_query(ctx, ntp_server, 1000, NC_CLOCK_ROUTE_AUTO, &ntp)) {
            char ntp_time[96];
            nc_format_unix_time_ns_ex(ntp.ntp_transmit_unix_ns, nc_cli_tz_mode, nc_cli_tz_offset_min, ntp_time, sizeof(ntp_time));
            printf("ntp_server=%s ntp_time=%s offset_ns=%lld delay_ns=%llu socket_setup_units=%llu send_recv_units=%llu migrated=%u stratum=%u\n",
                   ntp.server, ntp_time,
                   (long long)ntp.offset_ns,
                   (unsigned long long)ntp.delay_ns,
                   (unsigned long long)ntp.socket_setup_overhead_units,
                   (unsigned long long)ntp.send_recv_overhead_units,
                   ntp.migrated, ntp.stratum);
        } else {
            printf("ntp_server=%s status=unavailable_or_timeout\n", ntp_server);
        }
    }
}

static int nc_cli_run_precision_clock(int argc, char **argv, int live) {
    const char *ntp_server = NULL;
    int pin_cpu = 0; uint32_t cpu_index = 0;
    nc_cli_parse_clock_options(argc, argv, &ntp_server, &pin_cpu, &cpu_index);
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    if (pin_cpu) nc_clock_pin_thread_to_cpu(cpu_index);
    g_nc_stopwatch_stop = 0;
    signal(SIGINT, nc_cli_signal_handler);
#if defined(SIGTERM)
    signal(SIGTERM, nc_cli_signal_handler);
#endif
    do {
        nc_cli_print_precision_clock(ctx, ntp_server);
        if (!live) break;
        printf("---\n"); fflush(stdout);
        nc_cli_sleep_ms(nc_cli_detail == NC_CLOCK_DETAIL_NANO ? 100 : 250);
    } while (!g_nc_stopwatch_stop);
    nc_destroy(ctx);
    return 0;
}

static uint32_t nc_cli_parse_ms(int argc, char **argv, uint32_t def_ms) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (strcmp(argv[i], "--ms") == 0) return (uint32_t)strtoul(argv[i + 1], NULL, 10);
    }
    return def_ms;
}

static int nc_cli_run_stable_calibrate(int argc, char **argv) {
    int pin_cpu = 0; uint32_t cpu_index = 0;
    nc_cli_parse_clock_options(argc, argv, NULL, &pin_cpu, &cpu_index);
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    nc_stable_clock_config_t cfg;
    nc_stable_clock_default_config(&cfg);
    cfg.pin_cpu = (uint32_t)pin_cpu;
    cfg.cpu_index = cpu_index;
    cfg.calibration_ms = nc_cli_parse_ms(argc, argv, 500);
    nc_stable_clock_state_t st;
    if (!nc_calibrate_cycles_per_ns(ctx, &cfg, &st)) { nc_destroy(ctx); return 1; }
    printf("stable_calibration route=%s pinned=%u cpu_before=%u cpu_after=%u migrated=%u invariant_hint=%u\n",
           nc_clock_route_name((nc_clock_route_t)st.route), st.pinned, st.cpu_before, st.cpu_after, st.migrated, st.invariant_hint);
    printf("raw_start=%llu raw_end=%llu elapsed_units=%llu elapsed_ns=%llu\n",
           (unsigned long long)st.raw_start, (unsigned long long)st.raw_end,
           (unsigned long long)st.elapsed_units, (unsigned long long)st.elapsed_ns);
    printf("cycles_per_ns=%.12f ns_per_cycle=%.12f read_overhead_units=%llu kernel_overhead=%llu api_overhead=%llu\n",
           st.cycles_per_ns, st.ns_per_cycle,
           (unsigned long long)st.read_overhead_units,
           (unsigned long long)st.kernel_timecall_overhead_cycles,
           (unsigned long long)st.api_call_overhead_cycles);
    printf("formula: ns = cycles / cycles_per_ns\n");
    printf("advice: %s\n", nc_clock_stability_advice());
    nc_destroy(ctx);
    return 0;
}

static int nc_cli_run_cycles_to_ns(int argc, char **argv) {
    uint64_t cycles = 0; double cpn = 0.0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--cycles-to-ns") == 0 && i + 1 < argc) cycles = (uint64_t)strtoull(argv[++i], NULL, 10);
        else if (strcmp(argv[i], "--cycles-per-ns") == 0 && i + 1 < argc) cpn = strtod(argv[++i], NULL);
    }
    printf("cycles=%llu cycles_per_ns=%.12f ns=%llu\n",
           (unsigned long long)cycles, cpn,
           (unsigned long long)nc_cycles_to_ns_calibrated(cycles, cpn));
    return 0;
}

static int nc_cli_run_calibrate_clock(int argc, char **argv) {
    int pin_cpu = 0; uint32_t cpu_index = 0;
    nc_cli_parse_clock_options(argc, argv, NULL, &pin_cpu, &cpu_index);
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    nc_clock_calibration_result_t r;
    if (!nc_calibrate_clock_route(ctx, NC_CLOCK_ROUTE_AUTO, 200000, (uint32_t)pin_cpu, cpu_index, &r)) {
        nc_destroy(ctx); return 1;
    }
    printf("calibration route=%s samples=%llu pinned=%u cpu_before=%u cpu_after=%u migrated=%u\n",
           nc_clock_route_name((nc_clock_route_t)r.route), (unsigned long long)r.samples,
           r.pinned, r.cpu_before, r.cpu_after, r.migrated);
    printf("elapsed_raw=%llu elapsed_ns=%llu units_per_second=%.3f ns_per_unit=%.12f ppm_vs_ctx=%+.3f\n",
           (unsigned long long)r.elapsed_raw_units, (unsigned long long)r.elapsed_ns,
           r.units_per_second, r.ns_per_unit, r.ppm_error_vs_context);
    printf("overhead read=%llu kernel_timecall=%llu api_call=%llu\n",
           (unsigned long long)r.read_overhead_units,
           (unsigned long long)r.kernel_timecall_overhead_cycles,
           (unsigned long long)r.api_call_overhead_cycles);
    nc_destroy(ctx);
    return 0;
}

static void nc_cli_print_nanoclock_snapshot(nc_ctx_t *ctx) {
    nc_nanoclock_snapshot_t snap;
    char tbuf[64];
    memset(&snap, 0, sizeof(snap));
    if (!nc_nanoclock_snapshot(ctx, &snap)) {
        printf("nanoclock: snapshot failed\n");
        return;
    }
    nc_format_unix_time_ns(snap.unix_time_ns, tbuf, sizeof(tbuf));
    printf("time_utc=%s\n", tbuf);
    printf("monotonic_ns=%llu process_ns=%llu thread_ns=%llu\n",
           (unsigned long long)snap.monotonic_ns,
           (unsigned long long)snap.process_time_ns,
           (unsigned long long)snap.thread_time_ns);
    printf("selected_route=%s best_simd=%s backend=%s raw=%llu selected_ns=%llu overhead_units=%llu\n",
           nc_clock_route_name((nc_clock_route_t)snap.route),
           nc_asm_simd_family_name((nc_asm_simd_family_t)snap.best_simd_family),
           nc_backend_name(nc_backend(ctx)),
           (unsigned long long)snap.selected_raw_units,
           (unsigned long long)snap.selected_ns,
           (unsigned long long)snap.selected_overhead_units);
#if defined(__x86_64__) || defined(_M_X64)
    printf("x64_counters rdtsc_raw=%llu lfence_rdtsc=%llu mfence_lfence_rdtsc=%llu rdtscp_lfence=%llu aux=%u\n",
           (unsigned long long)snap.rdtsc_raw,
           (unsigned long long)snap.rdtsc_lfence,
           (unsigned long long)snap.rdtsc_mfence,
           (unsigned long long)snap.rdtscp_lfence,
           snap.rdtscp_aux);
#elif defined(__aarch64__) || defined(_M_ARM64)
    printf("arm64_counters cntfrq_el0=%llu cntvct_el0=%llu cntvct_isb=%llu cntvct_ns=%llu pmccntr_el0=%llu pmccntr_status=%u\n",
           (unsigned long long)snap.arm64_cntfrq_el0,
           (unsigned long long)snap.arm64_cntvct_el0,
           (unsigned long long)snap.arm64_cntvct_isb,
           (unsigned long long)snap.arm64_cntvct_ns,
           (unsigned long long)snap.arm64_pmccntr_el0,
           snap.arm64_pmccntr_status);
#endif
    printf("best_simd_counter=%llu best_simd_counter_ns=%llu\n",
           (unsigned long long)snap.best_simd_counter,
           (unsigned long long)snap.best_simd_counter_ns);
    printf("overhead native=%llu cyc ffi_baseline=%llu cyc dll=%llu cyc wrapper_hint=%llu cyc\n",
           (unsigned long long)snap.native_overhead_cycles,
           (unsigned long long)snap.ffi_overhead_cycles,
           (unsigned long long)snap.dll_boundary_cycles,
           (unsigned long long)snap.wrapper_hint_cycles);
}

static int nc_cli_run_ns_clock(int live) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    g_nc_stopwatch_stop = 0;
    signal(SIGINT, nc_cli_signal_handler);
#if defined(SIGTERM)
    signal(SIGTERM, nc_cli_signal_handler);
#endif
    do {
        nc_cli_print_nanoclock_snapshot(ctx);
        if (!live) break;
        printf("---\n");
        fflush(stdout);
        nc_cli_sleep_ms(250);
    } while (!g_nc_stopwatch_stop);
    nc_destroy(ctx);
    return 0;
}

static int nc_cli_run_wrapper_overhead(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    static const nc_wrapper_kind_t kinds[] = {
        NC_WRAPPER_C_API, NC_WRAPPER_DLL_BOUNDARY, NC_WRAPPER_PYTHON_CTYPES,
        NC_WRAPPER_PYTHON_CFFI, NC_WRAPPER_NODE_FFI, NC_WRAPPER_LUA_FFI,
        NC_WRAPPER_JAVA_JNA, NC_WRAPPER_CSHARP_PINVOKE, NC_WRAPPER_GO_CGO,
        NC_WRAPPER_RUST_FFI, NC_WRAPPER_ZIG_FFI
    };
    printf("Wrapper overhead native baselines. Interpreted wrappers should also run their own language-side loops.\n");
    for (size_t i = 0; i < sizeof(kinds)/sizeof(kinds[0]); ++i) {
        uint64_t cyc = nc_measure_wrapper_overhead_cycles(ctx, kinds[i], 20000);
        printf("%-18s %10llu cyc %10llu ns\n", nc_wrapper_kind_name(kinds[i]),
               (unsigned long long)cyc,
               (unsigned long long)nc_cycles_to_ns(ctx, cyc));
    }
    nc_destroy(ctx);
    return 0;
}

static int nc_cli_run_catalog(void) {
    size_t n = nc_microbench_catalog_count();
    printf("NanoChronometer microbench catalog: %zu entries\n", n);
    for (size_t i = 0; i < n; ++i) {
        const nc_microbench_descriptor_t *d = nc_microbench_catalog_entry(i);
        printf("%4zu  %-20s arch=%-6s family=%-10s category=%-10s mode=%s available=%d\n",
               i, d->name, d->arch, d->family, d->category,
               nc_benchmark_mode_name(d->mode), nc_microbench_available(d));
    }
    return 0;
}

static int nc_cli_run_whitebox(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    uint8_t buffer[4096];
    memset(buffer, 0x5A, sizeof(buffer));
    for (size_t i = 0; i < nc_microbench_catalog_count(); ++i) {
        const nc_microbench_descriptor_t *d = nc_microbench_catalog_entry(i);
        if (d && nc_microbench_available(d)) {
            nc_microbench_result_t r;
            nc_microbench_run(ctx, i, 10000, buffer, sizeof(buffer), &r);
            printf("%s: cycles=%llu ns=%llu checksum=%llu\n", d->name,
                   (unsigned long long)r.cycles,
                   (unsigned long long)r.ns,
                   (unsigned long long)r.checksum);
            nc_destroy(ctx);
            return 0;
        }
    }
    nc_destroy(ctx);
    return 1;
}

static int nc_cli_run_sct_audit(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    nc_sidechannel_audit_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.samples = 512;
    cfg.input_len = 64;
    cfg.output_len = 32;
    nc_sidechannel_audit_result_t r;
    int ok = nc_sidechannel_audit_constant_time(ctx, nc_cli_candidate_ct, NULL, &cfg, &r);
    if (ok) {
        printf("side-channel timing audit demo: samples=%u fixed_mean=%llu random_mean=%llu t=%.3f leak=%s\n",
               r.samples,
               (unsigned long long)r.fixed_stats.mean_cycles,
               (unsigned long long)r.random_stats.mean_cycles,
               r.welch_t,
               r.likely_timing_leak ? "possible" : "not indicated");
    }
    nc_destroy(ctx);
    return ok ? 0 : 1;
}

static int nc_cli_run_asm_probe(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) return 1;
    uint64_t lane[8];
    for (size_t i = 0; i < 8; ++i) lane[i] = 0x1111111111111111ull * (uint64_t)(i + 1u);
    nc_asm_sidechannel_probe_result_t load_r, reload_r, prefetch_r, barrier_r;
    nc_asm_sidechannel_cache_audit(ctx, lane, &load_r, &reload_r, &prefetch_r);
    nc_asm_sidechannel_probe(ctx, NC_ASM_SC_PROBE_BARRIER, lane, 128, 0, &barrier_r);
    printf("asm probe arch=%d load=%llu units (%llu ns) reload_status=%d reload=%llu units prefetch=%llu units barrier=%llu units\n",
           nc_asm_sidechannel_arch(),
           (unsigned long long)load_r.raw_units,
           (unsigned long long)load_r.ns,
           reload_r.status,
           (unsigned long long)reload_r.raw_units,
           (unsigned long long)prefetch_r.raw_units,
           (unsigned long long)barrier_r.raw_units);
    nc_destroy(ctx);
    return 0;
}

static int nc_cli_run_once(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) {
        fprintf(stderr, "NanoChronometer: failed to create context\n");
        return 1;
    }

    nc_start(ctx);
    nc_sleep_us(ctx, 1000);

    char buf[64];
    nc_format_elapsed(ctx, buf, sizeof(buf));
    printf("NanoChronometer backend=%s hz=%llu elapsed=%s ffi_overhead=%llu cycles\n",
           nc_backend_name(nc_backend(ctx)),
           (unsigned long long)nc_tsc_hz(ctx),
           buf,
           (unsigned long long)nc_measure_ffi_overhead_cycles(ctx, 1000));

    nc_destroy(ctx);
    return 0;
}

static int nc_cli_run_stopwatch(void) {
    nc_ctx_t *ctx = nc_create();
    if (!ctx) {
        fprintf(stderr, "NanoChronometer: failed to create context\n");
        return 1;
    }

    g_nc_stopwatch_stop = 0;
    signal(SIGINT, nc_cli_signal_handler);
#if defined(SIGTERM)
    signal(SIGTERM, nc_cli_signal_handler);
#endif

    printf("NanoChronometer stopwatch\n");
    printf("backend=%s hz=%llu\n",
           nc_backend_name(nc_backend(ctx)),
           (unsigned long long)nc_tsc_hz(ctx));
    printf("Press Ctrl+C to stop.\n\n");

    nc_start(ctx);

    while (!g_nc_stopwatch_stop) {
        char buf[64];
        nc_format_elapsed(ctx, buf, sizeof(buf));
        printf("\relapsed=%s", buf);
        fflush(stdout);
        nc_cli_sleep_ms(25);
    }

    char final_buf[64];
    nc_format_elapsed(ctx, final_buf, sizeof(final_buf));
    printf("\relapsed=%s\n", final_buf);

    nc_destroy(ctx);
    return 0;
}



static void run_asm_simd_probe(nc_ctx_t *ctx) {
    unsigned char a[256], b[256], out[256];
    for (size_t i = 0; i < sizeof(a); ++i) { a[i] = (unsigned char)i; b[i] = (unsigned char)(0xa5u ^ i); out[i] = 0; }
    printf("ASM SIMD probes (available families only):\n");
    for (int fam = NC_ASM_SIMD_MMX; fam <= NC_ASM_SIMD_ARM64_SME; ++fam) {
        if (!nc_asm_simd_family_available((nc_asm_simd_family_t)fam)) continue;
        nc_asm_simd_probe_result_t load_r, xor_r, bar_r;
        nc_asm_simd_probe(ctx, (nc_asm_simd_family_t)fam, NC_ASM_SIMD_PROBE_VECTOR_LOAD, a, NULL, NULL, sizeof(a), 0, &load_r);
        nc_asm_simd_probe(ctx, (nc_asm_simd_family_t)fam, NC_ASM_SIMD_PROBE_VECTOR_XOR, a, b, out, sizeof(out), 0, &xor_r);
        nc_asm_simd_probe(ctx, (nc_asm_simd_family_t)fam, NC_ASM_SIMD_PROBE_BARRIER, NULL, NULL, NULL, 32, 0, &bar_r);
        printf("  %-16s vector_load=%llu raw (%llu ns) vector_xor=%llu raw (%llu ns) barrier=%llu raw (%llu ns)\n",
               nc_asm_simd_family_name((nc_asm_simd_family_t)fam),
               (unsigned long long)load_r.raw_units, (unsigned long long)load_r.ns,
               (unsigned long long)xor_r.raw_units, (unsigned long long)xor_r.ns,
               (unsigned long long)bar_r.raw_units, (unsigned long long)bar_r.ns);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return nc_cli_run_stopwatch();
    }

    if (argc == 2 &&
        (strcmp(argv[1], "--stopwatch") == 0 || strcmp(argv[1], "stopwatch") == 0)) {
        return nc_cli_run_stopwatch();
    }

    if (argc == 2 &&
        (strcmp(argv[1], "--once") == 0 || strcmp(argv[1], "--info") == 0 ||
         strcmp(argv[1], "once") == 0 || strcmp(argv[1], "info") == 0)) {
        return nc_cli_run_once();
    }


    if (argc == 2 && (strcmp(argv[1], "--catalog") == 0 || strcmp(argv[1], "catalog") == 0)) {
        return nc_cli_run_catalog();
    }

    if (argc == 2 && (strcmp(argv[1], "--whitebox") == 0 || strcmp(argv[1], "whitebox") == 0)) {
        return nc_cli_run_whitebox();
    }

    if (argc == 2 && (strcmp(argv[1], "--sct-audit") == 0 || strcmp(argv[1], "sct-audit") == 0)) {
        return nc_cli_run_sct_audit();
    }

    if (argc == 2 && (strcmp(argv[1], "--asm-probe") == 0 || strcmp(argv[1], "asm-probe") == 0)) {
        return nc_cli_run_asm_probe();
    }

    if (argc == 2 && (strcmp(argv[1], "--asm-simd") == 0 || strcmp(argv[1], "asm-simd") == 0)) {
        nc_ctx_t *ctx = nc_create();
        if (!ctx) return 1;
        run_asm_simd_probe(ctx);
        nc_destroy(ctx);
        return 0;
    }

    if (argc >= 2 && (strcmp(argv[1], "--clock") == 0 || strcmp(argv[1], "clock") == 0)) {
        return nc_cli_run_precision_clock(argc, argv, 1);
    }
    if (argc >= 2 && (strcmp(argv[1], "--clock-once") == 0 || strcmp(argv[1], "clock-once") == 0)) {
        return nc_cli_run_precision_clock(argc, argv, 0);
    }
    if (argc >= 2 && (strcmp(argv[1], "--stable-calibrate") == 0 || strcmp(argv[1], "stable-calibrate") == 0)) {
        return nc_cli_run_stable_calibrate(argc, argv);
    }
    if (argc >= 2 && strcmp(argv[1], "--cycles-to-ns") == 0) {
        return nc_cli_run_cycles_to_ns(argc, argv);
    }
    if (argc >= 2 && (strcmp(argv[1], "--calibrate-clock") == 0 || strcmp(argv[1], "calibrate-clock") == 0)) {
        return nc_cli_run_calibrate_clock(argc, argv);
    }
    if (argc >= 3 && strcmp(argv[1], "--ntp") == 0) {
        return nc_cli_run_precision_clock(argc, argv, 0);
    }

    if (argc == 2 && (strcmp(argv[1], "--ns-clock") == 0 || strcmp(argv[1], "ns-clock") == 0)) {
        return nc_cli_run_ns_clock(1);
    }

    if (argc == 2 && (strcmp(argv[1], "--ns-clock-once") == 0 || strcmp(argv[1], "ns-clock-once") == 0)) {
        return nc_cli_run_ns_clock(0);
    }

    if (argc == 2 && (strcmp(argv[1], "--wrapper-overhead") == 0 || strcmp(argv[1], "wrapper-overhead") == 0)) {
        return nc_cli_run_wrapper_overhead();
    }

    if (argc == 2 &&
        (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 ||
         strcmp(argv[1], "help") == 0)) {
        nc_cli_usage(argv[0]);
        return 0;
    }

    fprintf(stderr, "Unknown argument: %s\n\n", argv[1]);
    nc_cli_usage(argv[0]);
    return 2;
}
