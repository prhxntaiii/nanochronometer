/*
 * nanochrono_wasm.c — NanoChronometer WASM/WASI adapter layer
 *
 * Compilation targets:
 *   Emscripten:  emcc nanochrono_wasm.c -o nanochrono.js -s WASM=1 \
 *                  -s EXPORTED_FUNCTIONS='["_nc_wasm_now_ns","_nc_wasm_feature_available","_nc_wasm_version"]' \
 *                  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'
 *   WASI-SDK:    /opt/wasi-sdk/bin/clang --target=wasm32-wasi nanochrono_wasm.c -o nanochrono.wasm
 *
 * Design rules (Boolean sandbox constraints):
 *   - WASM sandbox has no RDTSC, no CNTPCT_EL0, no kernel modules.
 *   - All hardware benchmarks return NC_ERR_UNSUPPORTED (caller prints "N/A").
 *   - Timing available only via WASI clock_time_get (monotonic wall clock).
 *   - No assembly sources compiled into WASM target.
 *   - nc_wasm_feature_available() is the single authoritative probe for
 *     all language bindings running under WASM/WASI.
 *
 * The dispatcher table has exactly one entry: wasi_monotonic (always passes).
 * All hardware/crypto/SIMD entries are elided at compile time (#ifndef __wasm__).
 */

#ifdef __wasm__

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ── WASI clock ─────────────────────────────────────────────────────────── */
#if defined(__wasi__)
#  include <wasi/api.h>

static uint64_t nc_wasi_monotonic_ns_(void)
{
    __wasi_timestamp_t t = 0;
    __wasi_clock_time_get(__WASI_CLOCKID_MONOTONIC, 1 /* 1 ns precision */, &t);
    return (uint64_t)t;
}

static uint64_t nc_wasi_realtime_ns_(void)
{
    __wasi_timestamp_t t = 0;
    __wasi_clock_time_get(__WASI_CLOCKID_REALTIME, 1, &t);
    return (uint64_t)t;
}

#else /* Emscripten / browser WASM — use JS Date.now() via import or emscripten_get_now */
#  if defined(__EMSCRIPTEN__)
#    include <emscripten.h>
static uint64_t nc_wasi_monotonic_ns_(void)
{
    double ms = emscripten_get_now();
    return (uint64_t)(ms * 1e6);
}
static uint64_t nc_wasi_realtime_ns_(void)
{
    return nc_wasi_monotonic_ns_();
}
#  else
/* Bare WASM without WASI or Emscripten — no timing available. */
static uint64_t nc_wasi_monotonic_ns_(void){ return 0; }
static uint64_t nc_wasi_realtime_ns_(void) { return 0; }
#  endif
#endif

/* ── Minimal opaque context ─────────────────────────────────────────────── */
typedef struct nc_wasm_ctx {
    uint64_t start_ns;
    uint64_t overhead_ns;
} nc_wasm_ctx_t;

static nc_wasm_ctx_t s_ctx_;
static int s_ctx_init_ = 0;

static nc_wasm_ctx_t *nc_wasm_ctx_(void)
{
    if (!s_ctx_init_) {
        memset(&s_ctx_, 0, sizeof(s_ctx_));
        s_ctx_init_ = 1;
    }
    return &s_ctx_;
}

/* ── Public API ─────────────────────────────────────────────────────────── */

/* Current monotonic time in nanoseconds (WASI-only precision). */
uint64_t nc_wasm_now_ns(void)
{
    return nc_wasi_monotonic_ns_();
}

/* Wall-clock (Unix epoch) time in nanoseconds. */
uint64_t nc_wasm_realtime_ns(void)
{
    return nc_wasi_realtime_ns_();
}

/* Start a measurement. Returns the start timestamp. */
uint64_t nc_wasm_start(void)
{
    nc_wasm_ctx_t *c = nc_wasm_ctx_();
    c->start_ns = nc_wasi_monotonic_ns_();
    return c->start_ns;
}

/* Return elapsed ns since last nc_wasm_start(). */
uint64_t nc_wasm_elapsed_ns(void)
{
    nc_wasm_ctx_t *c = nc_wasm_ctx_();
    uint64_t now = nc_wasi_monotonic_ns_();
    return (now >= c->start_ns) ? (now - c->start_ns) : 0;
}

/*
 * nc_wasm_feature_available(name) — runtime feature probe.
 *
 * Returns 1 if the feature is available in this WASM/WASI sandbox,
 * 0 if not (caller should report "N/A").
 *
 * Boolean rule: WASM sandbox excludes everything hardware-specific.
 *   available(f) = f ∈ {"timing.monotonic", "timing.realtime"}
 *   available(f) = 0  for all hardware, crypto, and SIMD features.
 */
int nc_wasm_feature_available(const char *name)
{
    if (!name) return 0;

    /* The only available features in WASM are the two WASI clocks. */
    if (strncmp(name, "timing.monotonic", 16) == 0) return 1;
    if (strncmp(name, "timing.realtime",  15) == 0) return 1;
    if (strncmp(name, "timing",            6) == 0) return 1;

    /* Everything else: hardware counters, crypto, SIMD — N/A in sandbox. */
    return 0;
}

/* Version string. */
const char *nc_wasm_version(void)
{
    return "NanoChronometer WASM 0.5.6";
}

/*
 * Stub implementations for the full C API surface.
 * These allow language bindings to call the same symbols whether targeting
 * native or WASM; the return value signals N/A via status == NC_ERR_UNSUPPORTED (-1).
 */

/* nc_measure_aesenc_cycles — N/A in WASM */
typedef struct { int status; uint32_t family, backend; uint64_t cycles, ns, blocks, checksum; } nc_wasm_instr_result_t;

uint64_t nc_wasm_measure_aesenc_cycles(uint32_t blocks, nc_wasm_instr_result_t *out)
{
    if (out) { memset(out, 0, sizeof(*out)); out->status = -1; out->blocks = blocks ? blocks : 1024; }
    return 0;
}

uint64_t nc_wasm_measure_sha256msg_cycles(uint32_t blocks, nc_wasm_instr_result_t *out)
{
    if (out) { memset(out, 0, sizeof(*out)); out->status = -1; out->blocks = blocks ? blocks : 1024; }
    return 0;
}

uint64_t nc_wasm_crypto_sha256_cycles(const void *msg, size_t len, void *out_digest_32, nc_wasm_instr_result_t *out)
{
    (void)msg; (void)len;
    if (out_digest_32) memset(out_digest_32, 0, 32);
    if (out) { memset(out, 0, sizeof(*out)); out->status = -1; }
    return 0;
}

uint64_t nc_wasm_crypto_rand_cycles(void *buf, size_t bytes, nc_wasm_instr_result_t *out)
{
#if defined(__wasi__)
    /* WASI has __wasi_random_get — use it if available. */
    extern __wasi_errno_t __wasi_random_get(uint8_t *buf, size_t buf_len);
    int ok = (__wasi_random_get((uint8_t*)buf, bytes) == 0);
    if (out) { memset(out, 0, sizeof(*out)); out->status = ok ? 0 : -3; out->blocks = bytes; }
    return 0;  /* no cycle count available */
#else
    if (buf) memset(buf, 0, bytes);
    if (out) { memset(out, 0, sizeof(*out)); out->status = -1; }
    return 0;
#endif
}

#endif /* __wasm__ */
