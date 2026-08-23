![Logo](assets/nanochronometer_logo.svg)

# NanoChronometer

High-resolution nanosecond stopwatch, microbenchmark suite, and timing library for Windows and Linux, targeting x64 and ARM64. The timing core reads hardware counters (TSC on x64, `CNTVCT_EL0` on ARM64) through hand-written assembly, calibrates them against monotonic system time, and exposes a C API, a Win32 GUI, and a CLI.

```
00:00:12:347:891:042
hh:mm:ss:mmm:uuu:nnn
```

---

## Repository layout

```
nanochrono.h / nanochrono.c       Public C API and high-level implementation
src/                               Core library sources
  nanochrono_runtime.c             Backend selection, calibration, TSC/counter reads
  nanochrono_instruction.c         Instruction-family dispatch and availability checks
  nanochrono_sidechannel.c         Defensive timing-audit and cache-probe API
  platform/
    android/nanochrono_android.c   Android privilege policy and backend selection
    windows/compat_msvcrt.c        MSVC CRT compatibility shim
    windows/nanochrono_arm64_x86_compat.c  x86 API stubs for ARM64 Windows
gui/                               Win32 GUI sources (Windows only)
  main.c / main_x64.c / main_arm64.c  GUI entry point (arch-selected by CMake)
  bench_kernels.c                  Scalar/SSE timer-family benchmark kernels
  bench_kernels_avx2.c             AVX/F16C/FMA/AVX2/AVX-512 benchmark kernels
  bench_kernels.h
  app.rc / resource.h              Win32 icon and resource wiring
cli/
  nanochrono_cli.c                 Command-line runner and CSV exporter
asm/
  x64/{linux,windows}/             NASM .asm files for every x64 ISA family
  arm64/{linux,windows,android}/   GNU .S files for scalar, NEON, SVE, SVE2, SME, SME2
externals/
  x64/{linux,windows}/{openssl,libsodium}/
  arm64/{linux,windows,android}/{boringssl,libsodium}/
assets/
  font/                            Drop a .ttf/.otf here for a custom GUI font
  nanochrono.ico
wrappers/                          Thin FFI wrappers: Rust, Go, C#, Java, Node.js, Zig, Lua
python/                            Python ctypes package (nanochronometer/)
scripts/                           Build helper scripts
```

---

## Building

### Prerequisites

- CMake 3.20 or later
- x64: NASM 2.15 or later
- ARM64: a compiler that accepts GNU assembly (GCC, Clang, or LLVM)
- Windows GUI: MSVC (Visual Studio 2019/2022) or MinGW/LLVM, opened from a 64-bit prompt

### CMake options

| Option | Default | Description |
|--------|---------|-------------|
| `NC_TARGET_ARCH` | `AUTO` | `AUTO`, `x64`, or `arm64` |
| `NC_TARGET_OS` | `AUTO` | `AUTO`, `windows`, `linux`, or `android` |
| `NC_CRYPTO_BACKEND` | `AUTO` | `AUTO`, `OPENSSL`, `BORINGSSL`, `LIBSODIUM`, `BOTH`, or `NONE` |
| `NC_BUILD_GUI` | `OFF` | Build the Win32 GUI executable |
| `NC_BUILD_CLI` | `ON` | Build the CLI executable |
| `NC_BUILD_SHARED` | `ON` | Build shared library |
| `NC_BUILD_STATIC` | `ON` | Build static library |
| `NC_ANDROID_LIBRARY_ONLY` | `ON` | Android NDK path: emit only `.so`/`.a`, no CLI |

### Windows — MSVC + Ninja (recommended)

From an x64 Native Tools Command Prompt:

```bat
cmake -S . -B build -G Ninja -DNC_CRYPTO_BACKEND=AUTO -DNC_BUILD_GUI=ON
cmake --build build
```

### Windows — MinGW + Ninja

```bat
cmake -S . -B build -G Ninja -DNC_CRYPTO_BACKEND=AUTO -DNC_BUILD_GUI=ON
cmake --build build
```

ARM64 with llvm-mingw:

```bat
set PATH=C:\llvm-mingw\bin;%PATH%
cmake -S . -B build -G Ninja -DNC_TARGET_ARCH=arm64 -DNC_BUILD_GUI=ON
cmake --build build
```

### Linux — x64

```sh
cmake -S . -B build -G "Unix Makefiles" -DNC_CRYPTO_BACKEND=AUTO
cmake --build build
```

### Linux — ARM64

```sh
cmake -S . -B build -G Ninja -DNC_TARGET_ARCH=arm64 -DNC_CRYPTO_BACKEND=AUTO
cmake --build build
```

### Android — NDK library (library only, no CLI)

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-24 \
  -DNC_TARGET_ARCH=arm64 -DNC_TARGET_OS=android \
  -DNC_ANDROID_LIBRARY_ONLY=ON
cmake --build build
```

### Android — Termux CLI

```sh
pkg install clang cmake ninja make
./scripts/build_termux_arm64_cli.sh
```

See [`TERMUX_ANDROID_CLI.md`](TERMUX_ANDROID_CLI.md) for install options.

### Build output layout

```
build/
  static/
    bin/   nanochrono_cli[.exe]  nanochrono_gui[.exe]
    lib/   libnanochrono.a  (or nanochrono.lib on MSVC)
    include/  nanochrono.h
  dynamic/
    lib/   libnanochrono.so  (or nanochrono.dll + import lib)
    include/  nanochrono.h
```

---

## C API quick start

```c
#include "nanochrono.h"

nc_ctx_t *ctx = nc_create();   // auto-selects best backend

nc_start(ctx);
/* ... work ... */
uint64_t ns = nc_elapsed_ns(ctx);

char buf[32];
nc_format_ns(ns, buf, sizeof(buf));
printf("%s\n", buf);   // e.g. "00:00:00:000:042:317"

nc_destroy(ctx);
```

Key API functions:

| Function | Description |
|----------|-------------|
| `nc_create()` / `nc_destroy()` | Allocate/free a timing context |
| `nc_start()` / `nc_elapsed_ns()` | Start timer and read elapsed nanoseconds |
| `nc_elapsed_us()` / `nc_elapsed_ms()` | Elapsed in microseconds / milliseconds |
| `nc_format_ns()` / `nc_format_elapsed()` | Format to `hh:mm:ss:mmm:uuu:nnn` string |
| `nc_sleep_us()` | Precision spin-sleep |
| `nc_tsc_hz()` | Calibrated TSC frequency |
| `nc_drift_ppm()` | TSC drift vs QPC/monotonic clock in PPM |
| `nc_nanoclock_snapshot()` | Full snapshot: wall, monotonic, raw counters, best route |
| `nc_measure_ffi_overhead_cycles()` | Measure FFI/wrapper call overhead |

The complete API is declared in `nanochrono.h`.

---

## GUI (Windows)

The Win32 GUI displays elapsed time in nanosecond format and includes a benchmark panel.

### Controls

| Key | Action |
|-----|--------|
| `Space` / `P` | Start, pause, resume |
| `S` | Stop |
| `R` | Reset |
| `B` | Show/hide benchmark panel |
| `M` | Toggle nano/simple display |
| `C` | Toggle digital/analog clock |
| `V` | Switch view (stopwatch / clock / timer) |
| `Z` | Toggle local/UTC |
| `ESC` | Exit |
| Drag | Move borderless window |

### Benchmark modes

- **Mode 1 — CPU/timer ISA:** runs NanoChronometer's own timer kernels across every supported instruction family (legacy scalar ASM, MMX, SSE through SSE4.2, AVX, F16C, FMA, AVX2, AVX-VNNI, AVX-512, AVX-512 VNNI). Rows unavailable on the current CPU are shown as `NOT AVAILABLE` and not executed.
- **Mode 2 — OpenSSL EVP:** AES-256-GCM, SHA-256, ChaCha20-Poly1305 through OpenSSL's EVP API.
- **Mode 3 — libsodium:** same algorithms through libsodium (`crypto_aead_aes256gcm_encrypt`, `crypto_hash_sha256`, ChaCha20-Poly1305-IETF).

Benchmark results can be exported as CSV from the GUI. For large sample counts use the CLI instead.

### Safe start / illegal instruction

If the GUI crashes on startup with an illegal instruction:

```bat
nanochrono_gui.exe --safe
```

or force a backend:

```bat
set NANOCHRONO_BACKEND=legacy
nanochrono_gui.exe
```

Valid `NANOCHRONO_BACKEND` values: `legacy`, `mmx`, `sse`, `sse2`, `sse3`, `ssse3`, `sse41`, `sse42`, `avx`, `f16c`, `fma`, `avx2`, `avx-vnni`, `avx512`, `avx512vnni`.

### Custom font

Drop a single `.ttf` or `.otf` file into `assets/font/`. The GUI loads it automatically at startup and scales it to fit the display. If the folder is empty, it falls back to Consolas → Courier New.

---

## CLI

```sh
# Single snapshot
nanochrono_cli --ns-clock-once

# Live clock (updates every 250 ms)
nanochrono_cli --ns-clock

# Large benchmark run, CSV output
nanochrono_cli --mode cpu --algo all --iterations 1000000 \
  --kernel-loops 1 --warmup 10000 --pin-core 2 \
  --priority high --csv results.csv

# NTP clock
nanochrono_cli --clock --nano --utc --ntp pool.ntp.org

# Stable calibration
nanochrono_cli --stable-calibrate --pin-cpu 0 --ms 1000

# Side-channel audit
nanochrono_cli --sct-audit

# List available white-box microbench entries
nanochrono_cli --catalog

# FFI wrapper overhead
nanochrono_cli --wrapper-overhead
```

---

## Timer backends vs. crypto backends

The nanosecond counter uses only timer backends — hardware cycle/tick counters. Crypto instruction extensions (AES-NI, SHA-NI, PCLMULQDQ, VAES) are **not** timer backends; they appear only in the OpenSSL/libsodium benchmark modes.

**Valid timer backends:** legacy scalar ASM, MMX, SSE, SSE2, SSE3, SSSE3, SSE4.1, SSE4.2, AVX, F16C, FMA, AVX2, AVX-VNNI, AVX-512, AVX-512 VNNI (x64); scalar, NEON, SVE, SVE2, SME, SME2 (ARM64).

---

## Accuracy note

NanoChronometer formats time down to nanoseconds, but OS scheduling, context switches, and frequency scaling affect measured values. TSC timing is most useful for short intervals and microbenchmarks on a pinned, isolated core. `QueryPerformanceCounter` (Windows) or `clock_gettime(CLOCK_MONOTONIC)` (Linux) remain the safest general-purpose clocks. NanoChronometer calibrates TSC against those clocks and reports drift in PPM to make the tradeoff visible.

For HFT / low-latency production use, see [`STABLE_CLOCK_CALIBRATION_HFT.md`](STABLE_CLOCK_CALIBRATION_HFT.md). For MiFID II / RTS 25 compliance, that document also lists the operational controls that must exist outside this library.

---

## Language wrappers

Thin FFI wrappers under `wrappers/` cover: **Rust**, **Go**, **C#/.NET**, **Java (JNA)**, **Node.js (ffi-napi)**, **Zig**, and **LuaJIT**. A Python `ctypes` package is in `python/nanochronometer/`.

All wrappers target the stable C ABI in `nanochrono.h`. Each wrapper's `overhead` example measures both the native timer overhead and the FFI/interpreter overhead on top of it.

See [`NANOCLOCK_AND_WRAPPER_OVERHEAD.md`](NANOCLOCK_AND_WRAPPER_OVERHEAD.md) for details.

---

## Further documentation

| File | Contents |
|------|----------|
| [`ASM_STABLE_CLOCK_NOTES.md`](ASM_STABLE_CLOCK_NOTES.md) | Exported ASM symbols for x64 and ARM64 raw counter reads |
| [`NANOCLOCK_AND_WRAPPER_OVERHEAD.md`](NANOCLOCK_AND_WRAPPER_OVERHEAD.md) | Nanosecond clock API and FFI overhead measurement |
| [`PRECISION_CLOCK_APP.md`](PRECISION_CLOCK_APP.md) | Clock-app layer: stopwatch, wall clock, NTP, CLI examples |
| [`STABLE_CLOCK_CALIBRATION_HFT.md`](STABLE_CLOCK_CALIBRATION_HFT.md) | HFT-style calibration flow and production notes |
| [`SIDECHANNEL_AUDIT.md`](SIDECHANNEL_AUDIT.md) | Defensive constant-time audit API, cache/load probes, white-box microbench catalog |
| [`UNIFIED_DISPATCH_AND_INSTRUCTION_TOOLKIT.md`](UNIFIED_DISPATCH_AND_INSTRUCTION_TOOLKIT.md) | Runtime dispatch model, instruction-family APIs, new ASM symbols |
| [`TERMUX_ANDROID_CLI.md`](TERMUX_ANDROID_CLI.md) | Native Android ARM64 CLI build inside Termux |
| [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md) | OpenSSL and libsodium license requirements for binary distribution |

---

## Crypto externals

Pre-built static libraries are included under `externals/` for convenience:

- x64: OpenSSL (`libcrypto.a` / `libcrypto.lib`) and libsodium
- ARM64: BoringSSL (OpenSSL is not used on ARM64) and libsodium

When linking the static OpenSSL archive on Windows, CMake automatically pulls in the required system libraries (`ws2_32`, `crypt32`, `bcrypt`, `advapi32`, `user32`). If distributing a binary that includes these libraries, include their license files — please see [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).

---

## License
This project is licensed under the **GNU General Public License v3.0 (GPLv3)**. See the [LICENSE](LICENSE) file for details.
