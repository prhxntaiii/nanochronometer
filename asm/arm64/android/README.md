# Android ARM64 ASM contract

This directory contains reusable ARM64 assembly kernels for Android arm64-v8a.
Do not split these files by average vs root.

These .S entry points are for NanoChronometer's non-crypto timing layer:

- CNTVCT_EL0 nanosecond/tick reads
- raw/serialized timer probes
- local side-channel and cache-latency probes
- NEON/SVE/SVE2/SME/SME2 microbench kernels
- barrier / ordering latency probes
- wrapper and interpreted-language latency measurement support

Privilege is not an ASM concern. Android user mode, Magisk/SuperSU consent,
perf_event_open, PMU availability, simpleperf import, and future privileged
helper processes are selected at runtime by src/platform/android/.

Crypto stays outside this directory and is delegated to BoringSSL/libsodium under
externals/arm64/android plus the C crypto backend.
