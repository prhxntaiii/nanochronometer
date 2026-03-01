# NanoChrono

High-resolution nanosecond stopwatch for Windows. Uses the CPU's Time Stamp Counter (TSC) directly via hand-written x86-64 assembly — no CRT, no runtime dependencies.

```
00:00:12:347:891:042
hh:mm:ss:ms :µs :ns
```

---

## Features

- **Nanosecond resolution** — reads TSC with LFENCE/CPUID serialization, calibrated against QPC
- **Dual ASM backends** — AVX (LFENCE + VMOVDQU) and Legacy (CPUID + SSE2), selected automatically at runtime via CPUID+XSAVE check
- **SIMD backbuffer clear** — 256-bit AVX or 128-bit SSE2 zeroing of the DIB section each frame
- **Custom font embedding** — drop any `.ttf` or `.otf` into `assets/font/` before building and it gets baked into the `.exe` as an RCDATA resource
- **Drift monitoring** — rolling QPC window tracks TSC drift in PPM, shown in the status bar
- **No CRT** — entry point is `WinMainCRTStartup`, links only `kernel32`, `user32`, `gdi32`, `dwmapi`, `winmm`
- **Borderless window** — drag anywhere to move, minimize/close buttons drawn in the title bar

## Controls

| Key | Action |
|-----|--------|
| `Space` / `P` | Start → Pause → Resume |
| `S` | Stop (keeps elapsed time) |
| `R` | Reset to `00:00:00:000:000:000` |
| `ESC` | Exit |
| Drag | Move window |

## Prerequisites

- **NASM** ≥ 2.15 — [nasm.us](https://www.nasm.us)
- **MSVC** — x64 Native Tools Command Prompt (VS 2019 or 2022)
- **Python 3** — for icon generation (stdlib only, optional)
- Windows 10 or later (requires `DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2`)

> **Platform:** x86-64 only. The ASM backends use instructions specific to the x86_64 TSC (`RDTSC`, `RDTSCP`, `CPUID`, `LFENCE`) and are **not compatible with 32-bit Windows or ARM64**.

## Building

Open an **x64 Native Tools Command Prompt** and run:

```bat
build_all.bat
```

Output goes to `dist\`:

```
dist\
  nanochrono_gui.exe          ← the stopwatch (both backends baked in)
  nanochrono.dll              ← Legacy DLL  (CPUID + SSE2)
  nanochrono_avx.dll          ← AVX DLL     (LFENCE + VMOVDQU)
  nanochrono_static.lib       ← Legacy static lib
  nanochrono_avx_static.lib   ← AVX static lib
```

### Build targets

```bat
build_all.bat           # full build (libs + GUI)
build_all.bat LIBS      # DLLs and static libs only
build_all.bat GUI       # GUI exe only (assumes libs already built)
build_all.bat CLEAN     # delete obj\ and dist\
build_all.bat REBUILD   # CLEAN + full build
```

### Custom font

Place a single `.ttf` or `.otf` file in `assets/font/` before building. The build script detects it automatically, compiles it as an RCDATA resource, and links it into the exe. No external files needed at runtime.

```
assets\
  font\
    YourFont.ttf    ← put it here
```

If the folder is empty, the GUI falls back to **Consolas → Courier New**.

To check the exact GDI family name that will be used:

```
python check_font_name.py assets\font\YourFont.ttf
```

## C API

The timing library (`nanochrono.h` / `nanochrono.c`) is usable independently of the GUI.

```c
#include "nanochrono.h"

// High-level API — automatic backend selection
nc_ctx_t *ctx = nc_create();       // calibrates TSC on creation (~500 ms)

nc_start(ctx);
// ... work ...
uint64_t ns = nc_elapsed_ns(ctx);  // nanoseconds since nc_start()

char buf[32];
nc_format_ns(ns, buf, sizeof(buf)); // "00:00:00:000:001:234"

nc_destroy(ctx);
```

### Linking options

| Use case | What to link |
|----------|-------------|
| Static, best backend at runtime | `nc_legacy.obj` + `nc_avx.obj` + `nc_c_uni.obj` |
| Static, legacy only | `nc_legacy.obj` + `nc_c_leg.obj` |
| Static, AVX only | `nc_avx.obj` + `nc_c_avx.obj` |
| Dynamic, auto | `nanochrono.lib` + `nanochrono_avx.lib` |

### Key API

```c
// Lifecycle
nc_ctx_t *nc_create(void);
nc_ctx_t *nc_create_backend(nc_backend_t backend);  // NC_BACKEND_LEGACY or NC_BACKEND_AVX
void      nc_destroy(nc_ctx_t *ctx);
void      nc_reset(nc_ctx_t *ctx);

// Timing
uint64_t  nc_start(nc_ctx_t *ctx);
uint64_t  nc_elapsed_ns(nc_ctx_t *ctx);
uint64_t  nc_elapsed_us(nc_ctx_t *ctx);
uint64_t  nc_elapsed_ms(nc_ctx_t *ctx);
double    nc_elapsed_sec(nc_ctx_t *ctx);

// Spin-sleep (TSC-based, not OS scheduler)
void      nc_sleep_ns(nc_ctx_t *ctx, uint64_t ns);

// Info
uint64_t  nc_tsc_hz(nc_ctx_t *ctx);      // calibrated frequency
double    nc_drift_ppm(nc_ctx_t *ctx);   // rolling drift estimate
nc_backend_t nc_backend(nc_ctx_t *ctx);

// Formatting
void nc_format_ns(uint64_t ns, char *buf, size_t cap);  // "hh:mm:ss:mmm:uuu:nnn"
```

### Low-level ASM functions

Both backends expose raw TSC reads for microbenchmarking:

```c
// Legacy (CPUID serialized)
uint64_t nc_tsc_start(void);          // CPUID + RDTSC
uint64_t nc_tsc_end(void);            // RDTSCP + CPUID drain
uint64_t nc_tsc_overhead(void);       // round-trip cost in cycles

// AVX (LFENCE serialized)
uint64_t nc_avx_tsc_start(void);      // LFENCE + RDTSC
uint64_t nc_avx_tsc_end(void);        // RDTSCP + LFENCE
uint64_t nc_avx_tsc_overhead(void);

// SIMD memory (Legacy = SSE2 128-bit, AVX = YMM 256-bit)
void     nc_simd_zero_buf(void *dst, size_t bytes);
void     nc_avx_simd_zero_buf(void *dst, size_t bytes);
```

## Architecture

```
nanochrono_legacy.asm   — CPUID/RDTSC/SSE2 backend
nanochrono_avx.asm      — LFENCE/RDTSCP/AVX backend
nanochrono.c            — high-level C API, QPC calibration, drift tracking
main.c                  — Win32 GUI, font loading, SIMD backbuffer, state machine
```

The GUI links both ASM object files. At startup `nc_create()` reads CPUID leaf 1 and `_xgetbv(0)` to decide which backend to use — AVX if the OS has enabled YMM state save, Legacy otherwise.

## License

MIT

---

*Tested on Windows 10/11 x64, VS 2022, NASM 2.16.*
