#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif

/* =============================================================================
 * nanochrono_android.c
 *
 * Android ARM64 execution-context and capability layer.
 *
 * Important architecture rule:
 *   - ASM files (.S/.asm) are pure timing/probe kernels: CNTVCT nanosecond
 *     clock reads, side-channel/local latency probes, wrapper/interpreter
 *     latency probes, and other non-crypto microbenchmarks.
 *   - Root, Magisk/SuperSU consent, perf_event, PMU availability, and future
 *     privileged helper logic live here, not in ASM.
 *   - Crypto stays in C backends linked against libsodium/BoringSSL under
 *     externals/arm64/android.
 * ============================================================================= */

#include "nanochrono.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__ANDROID__)
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <sys/utsname.h>
#  include <sys/wait.h>
#  include <unistd.h>
#  include <time.h>
#  include <sys/syscall.h>
#  if defined(__has_include)
#    if __has_include(<sys/auxv.h>)
#      include <sys/auxv.h>
#      define NC_ANDROID_HAS_AUXV 1
#    endif
#    if __has_include(<asm/hwcap.h>)
#      include <asm/hwcap.h>
#    endif
#    if __has_include(<linux/perf_event.h>)
#      include <linux/perf_event.h>
#      define NC_ANDROID_HAS_PERF_EVENT_H 1
#    endif
#  endif
#endif

#ifndef NC_ANDROID_HAS_AUXV
#  define NC_ANDROID_HAS_AUXV 0
#endif
#ifndef NC_ANDROID_HAS_PERF_EVENT_H
#  define NC_ANDROID_HAS_PERF_EVENT_H 0
#endif

#ifndef HWCAP_ASIMD
#  define HWCAP_ASIMD (1UL << 1)
#endif
#ifndef HWCAP_AES
#  define HWCAP_AES (1UL << 3)
#endif
#ifndef HWCAP_PMULL
#  define HWCAP_PMULL (1UL << 4)
#endif
#ifndef HWCAP_SHA1
#  define HWCAP_SHA1 (1UL << 5)
#endif
#ifndef HWCAP_SHA2
#  define HWCAP_SHA2 (1UL << 6)
#endif
#ifndef HWCAP_CRC32
#  define HWCAP_CRC32 (1UL << 7)
#endif
#ifndef HWCAP_ATOMICS
#  define HWCAP_ATOMICS (1UL << 8)
#endif
#ifndef HWCAP_SVE
#  define HWCAP_SVE (1UL << 22)
#endif
#ifndef HWCAP2_SVE2
#  define HWCAP2_SVE2 (1UL << 1)
#endif
#ifndef HWCAP2_SME
#  define HWCAP2_SME (1UL << 23)
#endif
#ifndef HWCAP2_SME2
#  define HWCAP2_SME2 (1UL << 37)
#endif

#if defined(__ANDROID__) && !defined(__NR_perf_event_open) && defined(__aarch64__)
#  define __NR_perf_event_open 241
#endif

static void nc_android_copy_(char *dst, size_t cap, const char *src) {
    if (!dst || !cap) return;
    if (!src) src = "";
    snprintf(dst, cap, "%s", src);
}

NC_API const char *nc_android_privilege_name(nc_android_privilege_t privilege) {
    switch (privilege) {
    case NC_ANDROID_PRIVILEGE_USER_APP: return "user-app";
    case NC_ANDROID_PRIVILEGE_ROOT_PROMPTABLE: return "root-promptable";
    case NC_ANDROID_PRIVILEGE_ROOT_GRANTED: return "root-granted";
    case NC_ANDROID_PRIVILEGE_KERNEL_ASSISTED: return "kernel-assisted";
    case NC_ANDROID_PRIVILEGE_DENIED: return "denied";
    default: return "unknown";
    }
}

NC_API const char *nc_android_backend_name(nc_android_backend_t backend) {
    switch (backend) {
    case NC_ANDROID_BACKEND_NONE: return "none";
    case NC_ANDROID_BACKEND_USER_CNTVCT: return "android-user-cntvct";
    case NC_ANDROID_BACKEND_USER_PERF_EVENT: return "android-user-perf-event";
    case NC_ANDROID_BACKEND_ROOT_PERF_EVENT: return "android-root-perf-event-helper";
    case NC_ANDROID_BACKEND_SIMPLEPERF_IMPORT: return "android-simpleperf-import";
    case NC_ANDROID_BACKEND_KERNEL_ASSISTED: return "android-kernel-assisted";
    default: return "unknown";
    }
}

NC_API const char *nc_android_mode_name(nc_android_mode_t mode) {
    switch (mode) {
    case NC_ANDROID_MODE_AUTO: return "auto";
    case NC_ANDROID_MODE_USER_ONLY: return "user-only";
    case NC_ANDROID_MODE_ROOT_IF_CONSENTS: return "root-if-consents";
    case NC_ANDROID_MODE_REQUIRE_ROOT: return "require-root";
    default: return "unknown";
    }
}

NC_API int nc_android_default_config(nc_android_config_t *cfg) {
    if (!cfg) return NC_ERR_BAD_ARGUMENT;
    memset(cfg, 0, sizeof(*cfg));
    cfg->mode = NC_ANDROID_MODE_AUTO;
    cfg->allow_su_prompt = 0;          /* never prompt from auto mode */
    cfg->allow_pmu_counters = 1;
    cfg->allow_kernel_samples = 0;
    cfg->allow_system_wide_profile = 0;
    cfg->prefer_simpleperf = 0;
    cfg->sample_frequency_hz = 99;
    return NC_OK;
}

#if defined(__ANDROID__)
static int nc_android_file_exec_(const char *path) {
    return path && access(path, X_OK) == 0;
}

static int nc_android_command_exists_(const char *cmd) {
    char buf[160];
    if (!cmd || strlen(cmd) > 80) return 0;
    snprintf(buf, sizeof(buf), "command -v %s >/dev/null 2>&1", cmd);
    int rc = system(buf);
    return rc == 0;
}

static int nc_android_probe_su_promptable_(void) {
    if (nc_android_file_exec_("/system/bin/su")) return 1;
    if (nc_android_file_exec_("/system/xbin/su")) return 1;
    if (nc_android_file_exec_("/sbin/su")) return 1;
    if (nc_android_file_exec_("/vendor/bin/su")) return 1;
    if (nc_android_file_exec_("/debug_ramdisk/su")) return 1;
    if (nc_android_command_exists_("su")) return 1;
    return 0;
}

static int nc_android_probe_simpleperf_(void) {
    if (nc_android_file_exec_("/system/bin/simpleperf")) return 1;
    if (nc_android_file_exec_("/apex/com.android.runtime/bin/simpleperf")) return 1;
    if (nc_android_command_exists_("simpleperf")) return 1;
    return 0;
}

static void nc_android_cpuinfo_features_(nc_android_caps_t *out) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) return;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "Features") || strstr(line, "features")) {
            if (strstr(line, " asimd") || strstr(line, " neon")) out->has_neon = 1;
            if (strstr(line, " sve")) out->has_sve = 1;
            if (strstr(line, " sve2")) out->has_sve2 = 1;
            if (strstr(line, " sme")) out->has_sme = 1;
            if (strstr(line, " sme2")) out->has_sme2 = 1;
        }
    }
    fclose(f);
}

static void nc_android_detect_isa_(nc_android_caps_t *out) {
#  if defined(__aarch64__)
    out->has_cntvct_el0 = 1;
    out->has_neon = 1; /* AArch64 baseline includes Advanced SIMD in Android ABIs. */
#    if NC_ANDROID_HAS_AUXV
    unsigned long hwcap = getauxval(AT_HWCAP);
    unsigned long hwcap2 = getauxval(AT_HWCAP2);
    if (hwcap & HWCAP_ASIMD) out->has_neon = 1;
    if (hwcap & HWCAP_SVE) out->has_sve = 1;
    if (hwcap2 & HWCAP2_SVE2) out->has_sve2 = 1;
    if (hwcap2 & HWCAP2_SME) out->has_sme = 1;
    if (hwcap2 & HWCAP2_SME2) out->has_sme2 = 1;
#    endif
    nc_android_cpuinfo_features_(out);
#  endif
}

#  if NC_ANDROID_HAS_PERF_EVENT_H
static int nc_android_perf_open_(struct perf_event_attr *attr, pid_t pid, int cpu, int group_fd, unsigned long flags) {
#    ifdef __NR_perf_event_open
    return (int)syscall(__NR_perf_event_open, attr, pid, cpu, group_fd, flags);
#    else
    (void)attr; (void)pid; (void)cpu; (void)group_fd; (void)flags;
    errno = ENOSYS;
    return -1;
#    endif
}

static int nc_android_probe_perf_hw_(uint64_t config, int *restricted_out) {
    struct perf_event_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.type = PERF_TYPE_HARDWARE;
    attr.size = sizeof(attr);
    attr.config = config;
    attr.disabled = 1;
    attr.exclude_kernel = 1;
    attr.exclude_hv = 1;
    int fd = nc_android_perf_open_(&attr, 0, -1, -1, 0);
    if (fd >= 0) {
        close(fd);
        return 1;
    }
    if (errno == EACCES || errno == EPERM) {
        if (restricted_out) *restricted_out = 1;
    }
    return 0;
}
#  else
static int nc_android_probe_perf_hw_(uint64_t config, int *restricted_out) {
    (void)config;
    if (restricted_out) *restricted_out = 0;
    return 0;
}
#  endif

NC_API int nc_android_detect_capabilities(nc_android_caps_t *out) {
    if (!out) return NC_ERR_BAD_ARGUMENT;
    memset(out, 0, sizeof(*out));
    out->struct_size = (uint32_t)sizeof(*out);
    out->privilege = NC_ANDROID_PRIVILEGE_USER_APP;
    out->recommended_backend = NC_ANDROID_BACKEND_USER_CNTVCT;
    nc_android_copy_(out->abi, sizeof(out->abi), "arm64-v8a");

    struct utsname un;
    if (uname(&un) == 0) {
        nc_android_copy_(out->kernel_release, sizeof(out->kernel_release), un.release);
        nc_android_copy_(out->device_model, sizeof(out->device_model), un.machine);
    }

    if (geteuid() == 0) {
        out->current_process_root = 1;
        out->root_granted = 1;
        out->privilege = NC_ANDROID_PRIVILEGE_ROOT_GRANTED;
    }

    nc_android_detect_isa_(out);
    out->root_prompt_available = nc_android_probe_su_promptable_();
    if (out->root_prompt_available && !out->root_granted) {
        out->privilege = NC_ANDROID_PRIVILEGE_ROOT_PROMPTABLE;
    }
    out->simpleperf_available = nc_android_probe_simpleperf_();

#  if NC_ANDROID_HAS_PERF_EVENT_H
    int restricted = 0;
    out->perf_event_available = nc_android_probe_perf_hw_(PERF_COUNT_HW_CPU_CYCLES, &restricted);
    out->perf_event_restricted = restricted;
    out->perf_hw_cycles_available = out->perf_event_available;
    out->perf_instructions_available = nc_android_probe_perf_hw_(PERF_COUNT_HW_INSTRUCTIONS, &restricted);
    if (restricted) out->perf_event_restricted = 1;
    out->perf_cache_misses_available = nc_android_probe_perf_hw_(PERF_COUNT_HW_CACHE_MISSES, &restricted);
    if (restricted) out->perf_event_restricted = 1;
    out->perf_branch_misses_available = nc_android_probe_perf_hw_(PERF_COUNT_HW_BRANCH_MISSES, &restricted);
    if (restricted) out->perf_event_restricted = 1;
#  endif

    if (out->perf_event_available) {
        out->recommended_backend = NC_ANDROID_BACKEND_USER_PERF_EVENT;
    } else if (out->simpleperf_available) {
        out->recommended_backend = NC_ANDROID_BACKEND_SIMPLEPERF_IMPORT;
    } else {
        out->recommended_backend = NC_ANDROID_BACKEND_USER_CNTVCT;
    }
    return NC_OK;
}

NC_API int nc_android_request_root_consent(nc_android_root_consent_result_t *out) {
    nc_android_root_consent_result_t local;
    if (!out) out = &local;
    memset(out, 0, sizeof(*out));
    out->struct_size = (uint32_t)sizeof(*out);

    if (geteuid() == 0) {
        out->prompt_attempted = 0;
        out->granted = 1;
        out->exit_code = 0;
        nc_android_copy_(out->message, sizeof(out->message), "current process already has uid=0; no Magisk/SuperSU prompt was needed");
        return NC_OK;
    }

    if (!nc_android_probe_su_promptable_()) {
        out->prompt_attempted = 0;
        out->granted = 0;
        out->exit_code = -1;
        nc_android_copy_(out->message, sizeof(out->message), "su not found; root consent cannot be requested");
        return NC_ERR_UNSUPPORTED;
    }

    /* Explicit user-consent point: this command is intentionally the first
     * privileged operation. Magisk/SuperSU should display its prompt here.
     * The library does not bypass, persist, or silently reuse privileged state. */
    out->prompt_attempted = 1;
    FILE *fp = popen("su -c id 2>&1", "r");
    if (!fp) {
        out->exit_code = errno ? errno : -1;
        nc_android_copy_(out->message, sizeof(out->message), "failed to execute su -c id");
        return NC_ERR_UNSUPPORTED;
    }

    char output[256];
    size_t used = 0;
    output[0] = '\0';
    while (used + 1 < sizeof(output) && fgets(output + used, (int)(sizeof(output) - used), fp)) {
        used = strlen(output);
    }
    int rc = pclose(fp);
    out->exit_code = rc;
    out->granted = (rc != -1 && WIFEXITED(rc) && WEXITSTATUS(rc) == 0 && strstr(output, "uid=0") != NULL) ? 1u : 0u;
    if (out->granted) {
        nc_android_copy_(out->message, sizeof(out->message), "root consent granted by su/Magisk/SuperSU");
        return NC_OK;
    }
    nc_android_copy_(out->message, sizeof(out->message), output[0] ? output : "root consent denied or su returned non-zero");
    return NC_ERR_UNSUPPORTED;
}

#else /* !__ANDROID__ */

NC_API int nc_android_detect_capabilities(nc_android_caps_t *out) {
    if (!out) return NC_ERR_BAD_ARGUMENT;
    memset(out, 0, sizeof(*out));
    out->struct_size = (uint32_t)sizeof(*out);
    out->privilege = NC_ANDROID_PRIVILEGE_DENIED;
    out->recommended_backend = NC_ANDROID_BACKEND_NONE;
    nc_android_copy_(out->abi, sizeof(out->abi), "not-android");
    return NC_ERR_UNSUPPORTED;
}

NC_API int nc_android_request_root_consent(nc_android_root_consent_result_t *out) {
    if (!out) return NC_ERR_BAD_ARGUMENT;
    memset(out, 0, sizeof(*out));
    out->struct_size = (uint32_t)sizeof(*out);
    out->granted = 0;
    nc_android_copy_(out->message, sizeof(out->message), "not an Android runtime");
    return NC_ERR_UNSUPPORTED;
}

#endif /* __ANDROID__ */

NC_API int nc_android_select_backend(const nc_android_config_t *cfg_in,
                                     const nc_android_caps_t *caps_in,
                                     nc_android_backend_info_t *out) {
    if (!out) return NC_ERR_BAD_ARGUMENT;
    memset(out, 0, sizeof(*out));
    out->struct_size = (uint32_t)sizeof(*out);

    nc_android_config_t cfg;
    if (cfg_in) cfg = *cfg_in;
    else nc_android_default_config(&cfg);

    nc_android_caps_t caps;
    if (caps_in) caps = *caps_in;
    else nc_android_detect_capabilities(&caps);

    out->privilege = caps.privilege;
    out->backend = NC_ANDROID_BACKEND_NONE;
    out->used_su = 0;
    out->user_granted_root = caps.root_granted;
    out->pmu_available = caps.perf_hw_cycles_available || caps.perf_cache_misses_available || caps.perf_branch_misses_available;
    out->pmu_restricted = caps.perf_event_restricted;

    if (cfg.mode == NC_ANDROID_MODE_USER_ONLY || cfg.mode == NC_ANDROID_MODE_AUTO) {
        if (cfg.allow_pmu_counters && caps.perf_event_available) {
            out->backend = NC_ANDROID_BACKEND_USER_PERF_EVENT;
            nc_android_copy_(out->reason, sizeof(out->reason), "using user-space perf_event backend");
        } else if (cfg.prefer_simpleperf && caps.simpleperf_available) {
            out->backend = NC_ANDROID_BACKEND_SIMPLEPERF_IMPORT;
            out->requires_helper_process = 1;
            nc_android_copy_(out->reason, sizeof(out->reason), "simpleperf is available for import/offline profiling");
        } else {
            out->backend = NC_ANDROID_BACKEND_USER_CNTVCT;
            out->fallback_used = caps.perf_event_restricted ? 1u : 0u;
            nc_android_copy_(out->reason, sizeof(out->reason), caps.perf_event_restricted ?
                             "perf_event/PMU restricted; using CNTVCT_EL0 nanosecond timer" :
                             "using CNTVCT_EL0 nanosecond timer");
        }
        return NC_OK;
    }

    if (cfg.mode == NC_ANDROID_MODE_ROOT_IF_CONSENTS || cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT) {
        int granted = caps.root_granted ? 1 : 0;
        if (!granted) {
            if (!cfg.allow_su_prompt) {
                out->backend = cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? NC_ANDROID_BACKEND_NONE : NC_ANDROID_BACKEND_USER_CNTVCT;
                out->fallback_used = cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? 0u : 1u;
                nc_android_copy_(out->reason, sizeof(out->reason),
                                 "root mode requested but allow_su_prompt is false; no Magisk/SuperSU prompt was invoked");
                return cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? NC_ERR_UNSUPPORTED : NC_OK;
            }
            nc_android_root_consent_result_t consent;
            int rc = nc_android_request_root_consent(&consent);
            out->used_su = consent.prompt_attempted;
            out->user_granted_root = consent.granted;
            granted = consent.granted ? 1 : 0;
            if (rc != NC_OK || !granted) {
                out->backend = cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? NC_ANDROID_BACKEND_NONE : NC_ANDROID_BACKEND_USER_CNTVCT;
                out->fallback_used = cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? 0u : 1u;
                nc_android_copy_(out->reason, sizeof(out->reason), consent.message);
                return cfg.mode == NC_ANDROID_MODE_REQUIRE_ROOT ? NC_ERR_UNSUPPORTED : NC_OK;
            }
        }

        out->privilege = NC_ANDROID_PRIVILEGE_ROOT_GRANTED;
        out->backend = NC_ANDROID_BACKEND_ROOT_PERF_EVENT;
        out->requires_helper_process = caps.current_process_root ? 0u : 1u;
        nc_android_copy_(out->reason, sizeof(out->reason),
                         caps.current_process_root ?
                         "current process is root; privileged perf_event backend may run in-process" :
                         "root consent granted; privileged perf_event backend requires a su/helper process");
        return NC_OK;
    }

    nc_android_copy_(out->reason, sizeof(out->reason), "unknown Android backend mode");
    return NC_ERR_BAD_ARGUMENT;
}
