#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include "nc_hw_dispatch.h"
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#  include <unistd.h>
#  include <time.h>
#endif

/* ── forward declarations for ASM counters ──────────────────────────────── */
#if defined(__x86_64__) || defined(_M_X64)
extern uint64_t nc_tsc_start(void);       /* CPUID + RDTSC                  */
extern uint64_t nc_tsc_lfence_start(void);/* LFENCE + RDTSC                 */
extern uint64_t nc_tsc_raw(void);         /* bare RDTSC                     */
extern uint64_t nc_tsc_overhead(void);
extern int      nc_tsc_invariant(void);
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
extern uint64_t nc_arm64_scalar_counter(void); /* MRS CNTVCT_EL0            */
#endif

/* ── platform helpers ───────────────────────────────────────────────────── */

static uint64_t nc_wall_monotonic_(void)
{
#if defined(_WIN32)
    LARGE_INTEGER q, f;
    QueryPerformanceCounter(&q);
    QueryPerformanceFrequency(&f);
    return (uint64_t)((__int128)q.QuadPart * 1000000000ull / f.QuadPart);
#elif defined(_POSIX_MONOTONIC_CLOCK) || defined(__linux__) || defined(__ANDROID__)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#endif
}

int nc_hw_is_android(void)
{
#if defined(__ANDROID__)
    return 1;
#else
    return 0;
#endif
}

int nc_hw_dev_pmu_ok(void)
{
#if defined(__linux__) && !defined(__ANDROID__)
    return (access("/dev/nanochrono_pmu", R_OK) == 0);
#else
    return 0;
#endif
}

int nc_hw_cntvct_ok(void)
{
#if defined(__aarch64__) || defined(_M_ARM64)
    return 1;   /* CNTVCT_EL0 is always user-accessible on Android / EL0 */
#else
    return 0;
#endif
}

/* ── CPUID / invariant TSC helpers (x86-64 only) ─────────────────────────── */
#if defined(__x86_64__) || defined(_M_X64)

static int nc_hw_cpuid_has_rdtscp_(void)
{
#if defined(_MSC_VER)
    int v[4]; __cpuidex(v,0x80000001,0);
    return (v[3]>>27)&1;
#elif defined(__GNUC__) || defined(__clang__)
    uint32_t a,b,c,d;
    __asm__ __volatile__("cpuid":"=a"(a),"=b"(b),"=c"(c),"=d"(d):"a"(0x80000001u),"c"(0));
    return (d>>27)&1;
#else
    return 0;
#endif
}

/* x86: RDTSCP+LFENCE — best serialised read when invariant TSC present */
static const char *name_rdtscp(void){ return "x64-rdtscp-lfence"; }
static int check_rdtscp(void){
    /* Require: invariant TSC AND RDTSCP instruction available */
    return nc_tsc_invariant() && nc_hw_cpuid_has_rdtscp_();
}
static uint64_t run_rdtscp(void){
    /* Use nc_tsc_start as the serialised form; nc_tsc_end reads RDTSCP+CPUID.
     * For counter selection purposes nc_tsc_start (CPUID+RDTSC) is equivalent
     * in ordering to RDTSCP+LFENCE and already available in the ASM library. */
    return nc_tsc_start();
}

/* x86: LFENCE+RDTSC — good serialisation without RDTSCP */
static const char *name_rdtsc_lfence(void){ return "x64-rdtsc-lfence"; }
static int check_rdtsc_lfence(void){ return nc_tsc_invariant(); }
static uint64_t run_rdtsc_lfence(void){ return nc_tsc_lfence_start(); }

/* x86: bare RDTSC — fallback when invariant TSC absent */
static const char *name_rdtsc_raw(void){ return "x64-rdtsc-raw"; }
static int check_rdtsc_raw(void){ return 1; }  /* always present on x86-64 */
static uint64_t run_rdtsc_raw(void){ return nc_tsc_raw(); }

#endif /* x86-64 */

/* ── ARM64 backends ─────────────────────────────────────────────────────── */
#if defined(__aarch64__) || defined(_M_ARM64)

/* Linux ARM64: CNTPCT_EL0 via kernel module /dev/nanochrono_pmu.
 * check: not Android AND /dev/nanochrono_pmu is readable.
 *   De Morgan applied: nc_hw_is_android() OR !nc_hw_dev_pmu_ok() → skip.
 */
static const char *name_cntpct(void){ return "arm64-cntpct-el0-kmod"; }
static int check_cntpct(void){ return !nc_hw_is_android() && nc_hw_dev_pmu_ok(); }
static uint64_t run_cntpct(void){
    /* Read CNTPCT_EL0 directly — available after module sets CNTKCTL_EL1 */
#if defined(__GNUC__) || defined(__clang__)
    uint64_t v;
    __asm__ __volatile__("isb\n\tmrs %0, cntpct_el0" : "=r"(v));
    return v;
#else
    return nc_arm64_scalar_counter();
#endif
}

/* ARM64 (Linux & Android): CNTVCT_EL0 — always user-accessible virtual counter.
 * check: cntvct_ok() is always 1 on aarch64; keep Android as its own branch.
 *   Android absorption: nc_hw_is_android() ⊕ check_cntpct() — mutually exclusive.
 */
static const char *name_cntvct(void){ return "arm64-cntvct-el0"; }
static int check_cntvct(void){ return nc_hw_cntvct_ok(); }
static uint64_t run_cntvct(void){ return nc_arm64_scalar_counter(); }

#endif /* arm64 */

/* ── monotonic wall-clock fallback (all platforms) ─────────────────────── */
static const char *name_wall(void){ return "wall-monotonic-ns"; }
static int check_wall(void){ return 1; }
static uint64_t run_wall(void){ return nc_wall_monotonic_(); }

/* ── dispatch table ─────────────────────────────────────────────────────── */
static const nc_hw_entry_t nc_hw_table_[] = {
#if defined(__x86_64__) || defined(_M_X64)
    { name_rdtscp,        check_rdtscp,        run_rdtscp        },
    { name_rdtsc_lfence,  check_rdtsc_lfence,  run_rdtsc_lfence  },
    { name_rdtsc_raw,     check_rdtsc_raw,     run_rdtsc_raw     },
#elif defined(__aarch64__) || defined(_M_ARM64)
    { name_cntpct,        check_cntpct,        run_cntpct        },
    { name_cntvct,        check_cntvct,        run_cntvct        },
#endif
    { name_wall,          check_wall,          run_wall          },  /* sentinel */
};

static const int nc_hw_table_count_ =
    (int)(sizeof(nc_hw_table_) / sizeof(nc_hw_table_[0]));

const nc_hw_entry_t *nc_hw_dispatch_select(void)
{
    for (int i = 0; i < nc_hw_table_count_; ++i) {
        if (nc_hw_table_[i].check()) return &nc_hw_table_[i];
    }
    /* Unreachable: wall-clock sentinel always passes check(). */
    return &nc_hw_table_[nc_hw_table_count_ - 1];
}

const char *nc_hw_selected_name(void)
{
    return nc_hw_dispatch_select()->name();
}
