# NanoChronometer Android ARM64 backend architecture

NanoChronometer uses one reusable ARM64 ASM tree for Android:

```text
asm/arm64/android/*.S
```

There is intentionally no average/ vs root/ ASM split. The same kernels are used
for normal app/user execution and for privileged profiling modes. This keeps
benchmarks comparable and avoids drift.

## Layer split

```text
ASM kernel              what instruction/workload is measured
Timer/backend layer     CNTVCT_EL0, clock_gettime, perf_event, simpleperf
Privilege policy        user app, root promptable, root granted, helper needed
Profiler/analysis       normalized records, counters, timeline, reports
```

## Android modes

nc_android_mode_t defines runtime policy:

- NC_ANDROID_MODE_AUTO: never prompts for root; uses user perf_event if available,
  otherwise CNTVCT_EL0.
- NC_ANDROID_MODE_USER_ONLY: never prompts; user-mode only.
- NC_ANDROID_MODE_ROOT_IF_CONSENTS: calls the explicit root-consent gate only if
  the caller allowed it, then selects a root/helper PMU backend if granted.
- NC_ANDROID_MODE_REQUIRE_ROOT: same consent gate, but fails instead of falling
  back when root is denied.

Root consent is explicit: nc_android_request_root_consent() invokes su -c id,
which is intended to trigger the Magisk/SuperSU menu. NanoChronometer does not
bypass, silently elevate, or persist privileged access.

## Important limitation

A normal Android app process cannot become root just because a child su command
was granted. After consent, privileged PMU/system-wide profiling must run through
a root helper process, a su command channel, or a simpleperf/perf capture path.
The backend info reports requires_helper_process for this reason.

## What ASM is for

The .S and .asm files are NanoChronometer's nanosecond stopwatch and local
instrumentation kernels: side-channel probes, memory/cache latency, ordering
latency, interpreter/wrapper overhead, and ISA microbenchmarks. Crypto benchmark
workloads remain in the C crypto backend and link against BoringSSL/libsodium
from externals/arm64/android.

## Build target policy

The Android NDK path is library-only by default. It builds:

```text
dynamic/lib/libnanochrono.so
static/lib/libnanochrono.a
```

It does not build an APK, Gradle project, or GUI. Android app integration should
link the produced `.so`/`.a` from an external APK/Gradle project later.

A native Android CLI is still supported through Termux. Termux is treated as a
normal command-line host that uses the same Android ARM64 ASM kernels and the
same C runtime/backend policy layer:

```sh
scripts/build_termux_arm64_cli.sh
```

That script sets `-DNC_ANDROID_LIBRARY_ONLY=OFF -DNC_BUILD_CLI=ON`, so it emits
`static/bin/nanochrono_cli` plus the usual static/dynamic libraries.
