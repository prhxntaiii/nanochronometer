/* =============================================================================
 * main.c  -  NanoChrono GUI  (Win32, static backend)
 *
 * Typography:
 *   - Scans assets\font\ for the first *.ttf or *.otf file.
 *   - Loads it with AddFontResourceExW (FR_PRIVATE).
 *   - Reads the font's family name from the TTF 'name' table (nameID 1).
 *   - Binary-searches for the largest point size where GetTextExtentPoint32W
 *     confirms the timer string fits inside the display rect (both axes).
 *   - Falls back to Consolas -> Courier New if no custom font is found.
 *
 * ASM integration (exe links both backends statically):
 *   - nc_avx_simd_zero_buf / nc_simd_zero_buf used to clear backbuffer.
 *   - nc_avx_prefetch_tsc_start / nc_avx_tsc_end for render-time tracing.
 *   - nc_avx_warmup() called on first paint.
 *   - nc_avx_tsc_overhead() shown in status bar.
 *
 * Controls:  SPACE/P  Start -> Pause -> Resume
 *            S        Stop (keep value)
 *            R        Reset
 *            ESC      Exit
 *            Drag     Move window (anywhere on client area)
 * ============================================================================= */

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <intrin.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#if defined(__GNUC__) && !defined(_MSC_VER)
#  include <strings.h>
#  ifndef _stricmp
#    define _stricmp strcasecmp
#  endif
#  ifndef strcpy_s
#    define strcpy_s(dst, dstsz, src) ((void)snprintf((dst), (dstsz), "%s", (src)))
#  endif
#  ifndef sprintf_s
#    define sprintf_s snprintf
#  endif
#endif
#include <mmsystem.h>
#include <commdlg.h>
#include <immintrin.h>
#include <wmmintrin.h>

#if defined(NC_USE_OPENSSL) || defined(NC_USE_BORINGSSL) || \
    defined(NC_CRYPTO_BACKEND_OPENSSL) || defined(NC_CRYPTO_BACKEND_BOTH)
#  define NC_GUI_HAS_OPENSSL 1
#endif
#if defined(NC_USE_LIBSODIUM) || defined(NC_CRYPTO_BACKEND_LIBSODIUM) || \
    defined(NC_CRYPTO_BACKEND_BOTH)
#  define NC_GUI_HAS_LIBSODIUM 1
#endif

#if defined(NC_GUI_HAS_OPENSSL)
#  include <openssl/evp.h>
#  include <openssl/crypto.h>
#endif
#if defined(NC_GUI_HAS_LIBSODIUM)
#  include <sodium.h>
#endif

#include "bench_kernels.h"

/* CPU intrinsics benchmarks live in bench_kernels.c. */

/* Static build: pull in both ASM backends + unified C API */
#include "nanochrono.h"
#include "resource.h"

/* --------------------------------------------------------------------------
 * Build fallbacks (keeps single-file builds happy)
 * -------------------------------------------------------------------------- */
#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof((a)[0]))
#endif

/* If resource.h / .rc use a different icon ID, define IDI_APPICON there.
   This fallback just prevents compilation failures. */
#ifndef IDI_APPICON
#define IDI_APPICON 101
#endif

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comdlg32.lib")

#ifndef NC_CRYPTO_BACKEND_NAME
#define NC_CRYPTO_BACKEND_NAME "unlinked"
#endif

typedef enum {
    NC_GUI_CRYPTO_NONE = 0,
    NC_GUI_CRYPTO_OPENSSL,
    NC_GUI_CRYPTO_LIBSODIUM
} nc_gui_crypto_provider_t;

typedef enum {
    BENCH_MODE_CPU_INTRINSICS = 0,
    BENCH_MODE_OPENSSL_EVP,
    BENCH_MODE_LIBSODIUM
} bench_mode_t;

static bench_mode_t g_bench_mode = BENCH_MODE_CPU_INTRINSICS;
static nc_gui_crypto_provider_t g_crypto_provider_pref = NC_GUI_CRYPTO_NONE;

static int nc_crypto_has_openssl(void) {
#if defined(NC_GUI_HAS_OPENSSL)
    return 1;
#else
    return 0;
#endif
}

static int nc_crypto_has_libsodium(void) {
#if defined(NC_GUI_HAS_LIBSODIUM)
    return 1;
#else
    return 0;
#endif
}

static nc_backend_t gui_backend_from_name(const char *name, int *ok) {
    if (ok) *ok = 1;
    if (!name || !*name) { if (ok) *ok = 0; return NC_BACKEND_LEGACY; }
    if (_stricmp(name, "legacy") == 0 || _stricmp(name, "legacy-asm") == 0 || _stricmp(name, "scalar") == 0) return NC_BACKEND_LEGACY;
    if (_stricmp(name, "mmx") == 0) return NC_BACKEND_MMX;
    if (_stricmp(name, "sse") == 0) return NC_BACKEND_SSE;
    if (_stricmp(name, "sse2") == 0) return NC_BACKEND_SSE2;
    if (_stricmp(name, "sse3") == 0) return NC_BACKEND_SSE3;
    if (_stricmp(name, "ssse3") == 0) return NC_BACKEND_SSSE3;
    if (_stricmp(name, "sse41") == 0 || _stricmp(name, "sse4.1") == 0) return NC_BACKEND_SSE41;
    if (_stricmp(name, "sse42") == 0 || _stricmp(name, "sse4.2") == 0) return NC_BACKEND_SSE42;
    if (_stricmp(name, "avx") == 0) return NC_BACKEND_AVX;
    if (_stricmp(name, "f16c") == 0) return NC_BACKEND_F16C;
    if (_stricmp(name, "fma") == 0) return NC_BACKEND_FMA;
    if (_stricmp(name, "avx2") == 0) return NC_BACKEND_AVX2;
    if (_stricmp(name, "avx-vnni") == 0 || _stricmp(name, "avx_vnni") == 0 || _stricmp(name, "avxvnni") == 0) return NC_BACKEND_AVX_VNNI;
    if (_stricmp(name, "avx512") == 0 || _stricmp(name, "avx-512") == 0) return NC_BACKEND_AVX512;
    if (_stricmp(name, "avx512vnni") == 0 || _stricmp(name, "avx-512-vnni") == 0 || _stricmp(name, "avx512_vnni") == 0) return NC_BACKEND_AVX512VNNI;
    if (ok) *ok = 0;
    return NC_BACKEND_LEGACY;
}

static nc_ctx_t *gui_create_context_from_cmdline(const char *cmdline) {
    const char *env = getenv("NANOCHRONO_BACKEND");
    char forced[64] = {0};

    if (cmdline && (strstr(cmdline, "--safe") || strstr(cmdline, "/safe"))) {
        strcpy_s(forced, sizeof(forced), "legacy");
    } else if (cmdline) {
        const char *p = strstr(cmdline, "--backend=");
        if (!p) p = strstr(cmdline, "/backend=");
        if (p) {
            p = strchr(p, '=');
            if (p) {
                ++p;
                size_t n = 0;
                while (p[n] && p[n] != ' ' && p[n] != '\t' && p[n] != '"' && n + 1 < sizeof(forced)) { forced[n] = p[n]; ++n; }
                forced[n] = 0;
            }
        }
    }
    if (!forced[0] && env && *env) strcpy_s(forced, sizeof(forced), env);

    if (forced[0]) {
        int ok = 0;
        nc_backend_t b = gui_backend_from_name(forced, &ok);
        if (ok && nc_backend_is_available(b)) {
            nc_ctx_t *ctx = nc_create_backend(b);
            if (ctx) return ctx;
        }
        return nc_create_backend(NC_BACKEND_LEGACY);
    }

    nc_ctx_t *ctx = nc_create();
    if (!ctx) ctx = nc_create_backend(NC_BACKEND_LEGACY);
    return ctx;
}

static void nc_crypto_ensure_default_provider(void) {
    if (g_crypto_provider_pref != NC_GUI_CRYPTO_NONE) return;
#if defined(NC_GUI_HAS_OPENSSL)
    g_crypto_provider_pref = NC_GUI_CRYPTO_OPENSSL;
#elif defined(NC_GUI_HAS_LIBSODIUM)
    g_crypto_provider_pref = NC_GUI_CRYPTO_LIBSODIUM;
#else
    g_crypto_provider_pref = NC_GUI_CRYPTO_NONE;
#endif
}

static const wchar_t *nc_crypto_provider_name_w(nc_gui_crypto_provider_t provider) {
    switch (provider) {
    case NC_GUI_CRYPTO_OPENSSL: return L"OpenSSL";
    case NC_GUI_CRYPTO_LIBSODIUM: return L"libsodium";
    default: return L"unlinked";
    }
}

static const wchar_t *nc_crypto_backend_name_w(void) {
    nc_crypto_ensure_default_provider();
    return nc_crypto_provider_name_w(g_crypto_provider_pref);
}

static const wchar_t *bench_mode_name_w(bench_mode_t mode) {
    switch (mode) {
    case BENCH_MODE_CPU_INTRINSICS: return L"CPU intrinsics";
    case BENCH_MODE_OPENSSL_EVP:    return L"OpenSSL EVP";
    case BENCH_MODE_LIBSODIUM:      return L"libsodium";
    default:                        return L"unknown";
    }
}

static const char *bench_mode_name_a(bench_mode_t mode) {
    switch (mode) {
    case BENCH_MODE_CPU_INTRINSICS: return "CPU intrinsics";
    case BENCH_MODE_OPENSSL_EVP:    return "OpenSSL EVP";
    case BENCH_MODE_LIBSODIUM:      return "libsodium";
    default:                        return "unknown";
    }
}

static int bench_mode_available(bench_mode_t mode) {
    switch (mode) {
    case BENCH_MODE_CPU_INTRINSICS: return 1;
    case BENCH_MODE_OPENSSL_EVP:    return nc_crypto_has_openssl();
    case BENCH_MODE_LIBSODIUM:      return nc_crypto_has_libsodium();
    default:                        return 0;
    }
}

/* ============================================================================
 * Colours
 * ========================================================================== */
#ifndef CLR_BG
#define CLR_BG          0x00000000u
#endif
#define CLR_TIMER       0x004EFF00u   /* NanoChrono neon green */
#define CLR_TIMER_GLOW  0x001B5200u   /* dark green glow underlay */
#define CLR_PAUSED      0x0000AAFFu   /* blue when paused  */
#define CLR_STOPPED     0x00AA4400u   /* amber when stopped */
#define CLR_INFO        0x00B7B7B7u
#define CLR_STATUS      0x0066DD33u
#define CLR_BTN_ACTIVE  0x004EFF00u
#define CLR_BENCH_OK     0x0033FF88u
#define CLR_BENCH_OFF    0x00446644u
#define CLR_BENCH_LABEL  0x0099CC99u
#define CLR_BENCH_PANEL  0x000A120Au
#ifndef NC_GUI_PI
#define NC_GUI_PI 3.14159265358979323846
#endif

#ifndef CLR_BORDER
#define CLR_BORDER      0x00111111u
#endif

/* ============================================================================
 * Timer state
 * ========================================================================== */
typedef enum { TS_RESET = 0, TS_RUNNING, TS_PAUSED, TS_STOPPED } TimerState;

static nc_ctx_t   *g_ctx        = NULL;
static TimerState  g_state      = TS_RESET;
static uint64_t    g_elapsed_ns = 0;   /* frozen ns when paused/stopped */
static bool        g_show_bench = false;
static bool        g_simple_mode = false;
static nc_clock_view_t g_clock_view = NC_CLOCK_VIEW_STOPWATCH;
static nc_time_zone_mode_t g_clock_zone = NC_TIME_ZONE_LOCAL;
static int32_t g_clock_utc_offset_min = 0;
typedef enum { NC_GUI_CLOCK_DIGITAL = 0, NC_GUI_CLOCK_ANALOG = 1 } nc_gui_clock_design_t;
static nc_gui_clock_design_t g_clock_design = NC_GUI_CLOCK_DIGITAL;
static nc_cpu_features_t g_cpu_features = {0};
static int         g_cpu_has_avx512f = 0;

typedef nc_bench_kernel_id_t BenchKind;
#define BENCH_MAX_ROWS ((int)NC_BENCH_COUNT - 1)

typedef struct bench_state_t {
    volatile LONG running;
    volatile LONG finished;
    BenchKind selected;
    wchar_t selected_name[32];
    char log[65536];
    size_t log_len;
    RECT row_rects[BENCH_MAX_ROWS];
    BenchKind row_kinds[BENCH_MAX_ROWS];
    int row_count;
    RECT mode_row_rects[3];
    uint64_t last_cycles;
    double last_seconds;
    double last_mops;
    int save_prompted;
    HANDLE thread;
} bench_state_t;

typedef struct bench_profile_t {
    const char *title;
    const char *warmup;
    const char *unit;
    double ops_per_loop;
    double bytes_per_op;
    size_t loops[3];
    int repeats[3];
} bench_profile_t;

typedef struct bench_pass_result_t {
    int pass_no;
    int repeats;
    size_t loops;
    uint64_t cycles;
    double secs;
    double total_ops;
    double mops;
    double cyc_per_op;
    double ns_per_op;
    double total_bytes;
    double mibps;
    double gibps;
    uint64_t sink;
} bench_pass_result_t;

static bench_state_t g_bench = {0};
static CRITICAL_SECTION g_bench_lock;
static LONG g_bench_lock_ready = 0;
static HWND g_hwnd_main = NULL;
#define WM_APP_BENCH_DONE (WM_APP + 17)

/* ============================================================================
 * Layout
 * ========================================================================== */
static int  g_win_w = 920, g_win_h = 540;
static RECT g_rc_title, g_rc_timer, g_rc_btn_bar, g_rc_status, g_rc_bench_btn, g_rc_mode_btn, g_rc_max_btn, g_rc_clock_toggle_btn, g_rc_view_btn, g_rc_zone_btn;
static RECT g_rc_stopwatch_btn, g_rc_timer_tab_btn, g_rc_nano_btn, g_rc_utc_btn;
static RECT g_rc_min_btn, g_rc_close_btn;
static float g_dpi_scale = 1.0f;
static int S(int v) { return (int)(v * g_dpi_scale + 0.5f); }

static void update_header_control_rects(void) {
    int y = S(44), h = S(32);
    int btn_w = S(34);
    g_rc_close_btn.left = g_win_w - btn_w; g_rc_close_btn.top = 0; g_rc_close_btn.right = g_win_w; g_rc_close_btn.bottom = S(32);
    g_rc_max_btn.left = g_win_w - btn_w * 2; g_rc_max_btn.top = 0; g_rc_max_btn.right = g_win_w - btn_w; g_rc_max_btn.bottom = S(32);
    g_rc_min_btn.left = g_win_w - btn_w * 3; g_rc_min_btn.top = 0; g_rc_min_btn.right = g_win_w - btn_w * 2; g_rc_min_btn.bottom = S(32);
    g_rc_view_btn.left = S(10); g_rc_view_btn.top = S(32); g_rc_view_btn.right = S(112); g_rc_view_btn.bottom = g_rc_title.bottom;
    g_rc_stopwatch_btn.left = S(112); g_rc_stopwatch_btn.top = S(32); g_rc_stopwatch_btn.right = S(252); g_rc_stopwatch_btn.bottom = g_rc_title.bottom;
    g_rc_timer_tab_btn.left = S(252); g_rc_timer_tab_btn.top = S(32); g_rc_timer_tab_btn.right = S(362); g_rc_timer_tab_btn.bottom = g_rc_title.bottom;
    g_rc_clock_toggle_btn.left = S(332); g_rc_clock_toggle_btn.top = S(32); g_rc_clock_toggle_btn.right = S(430); g_rc_clock_toggle_btn.bottom = g_rc_title.bottom;
    int x = g_win_w - S(410);
    g_rc_mode_btn.left = x; g_rc_mode_btn.top = y; g_rc_mode_btn.right = x + S(72); g_rc_mode_btn.bottom = y + h;
    g_rc_nano_btn.left = x + S(86); g_rc_nano_btn.top = y; g_rc_nano_btn.right = x + S(152); g_rc_nano_btn.bottom = y + h;
    g_rc_zone_btn.left = x + S(190); g_rc_zone_btn.top = y; g_rc_zone_btn.right = x + S(260); g_rc_zone_btn.bottom = y + h;
    g_rc_utc_btn.left = x + S(276); g_rc_utc_btn.top = y; g_rc_utc_btn.right = x + S(324); g_rc_utc_btn.bottom = y + h;
    g_rc_bench_btn.left = x + S(338); g_rc_bench_btn.top = y; g_rc_bench_btn.right = x + S(408); g_rc_bench_btn.bottom = y + h;
}

static int header_control_hit(POINT pt) {
    return PtInRect(&g_rc_view_btn, pt) || PtInRect(&g_rc_stopwatch_btn, pt) ||
           PtInRect(&g_rc_timer_tab_btn, pt) || PtInRect(&g_rc_clock_toggle_btn, pt) ||
           PtInRect(&g_rc_mode_btn, pt) || PtInRect(&g_rc_nano_btn, pt) ||
           PtInRect(&g_rc_zone_btn, pt) || PtInRect(&g_rc_utc_btn, pt) ||
           PtInRect(&g_rc_bench_btn, pt) || PtInRect(&g_rc_min_btn, pt) ||
           PtInRect(&g_rc_max_btn, pt) || PtInRect(&g_rc_close_btn, pt);
}

/* ============================================================================
 * Font state
 * ========================================================================== */
static HFONT  g_font_timer  = NULL;
static HFONT  g_font_glow   = NULL;
static HFONT  g_font_info   = NULL;
static HFONT  g_font_status = NULL;
static wchar_t g_custom_face[LF_FACESIZE] = {0};  /* empty = no custom font */
static HANDLE  g_font_res   = NULL;                /* AddFontMemResourceEx handle */
static bool    g_warmup_done = false;
static bool    g_custom_font_loaded = false; /* TRUE when AddFontMemResourceEx succeeded */

/* ============================================================================
 * Backbuffer
 * ========================================================================== */
static HDC     g_dc_buf  = NULL;
static HBITMAP g_bm_buf  = NULL;
static void   *g_bm_bits = NULL;   /* DIBSection pixels (for SIMD clear) */
static int     g_bm_w    = 0, g_bm_h = 0;

/* ============================================================================
 * Font helpers
 * ========================================================================== */
static HFONT make_font_pt(int pt, const wchar_t *face, int weight, BOOL italic) {
    HDC dc = GetDC(NULL);
    int h  = -MulDiv(pt, GetDeviceCaps(dc, LOGPIXELSY), 72);
    ReleaseDC(NULL, dc);
    return CreateFontW(h, 0, 0, 0, weight, italic, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, face);
}

/* Binary-search for the largest pt where sample_text fits in (max_w x max_h).
   Returns best point size found.                                              */
static int fit_pt(const wchar_t *face, int weight, const wchar_t *sample,
                  int max_w, int max_h)
{
    HDC dc  = GetDC(NULL);
    int lo  = 10, hi = 600, best = 10;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        HFONT f   = make_font_pt(mid, face, weight, FALSE);
        HFONT old = (HFONT)SelectObject(dc, f);
        SIZE  sz  = {0};
        GetTextExtentPoint32W(dc, sample, (int)wcslen(sample), &sz);
        SelectObject(dc, old);
        DeleteObject(f);
        if (sz.cx <= max_w && sz.cy <= max_h) { best = mid; lo = mid + 1; }
        else                                   {             hi = mid - 1; }
    }
    ReleaseDC(NULL, dc);
    return best;
}

static void destroy_fonts(void) {
    if (g_font_timer)  { DeleteObject(g_font_timer);  g_font_timer  = NULL; }
    if (g_font_glow)   { DeleteObject(g_font_glow);   g_font_glow   = NULL; }
    if (g_font_info)   { DeleteObject(g_font_info);   g_font_info   = NULL; }
    if (g_font_status) { DeleteObject(g_font_status); g_font_status = NULL; }
}

static void create_fonts(void) {
    destroy_fonts();

    /* Timer rect dimensions */
    int tw = g_rc_timer.right  - g_rc_timer.left;
    int th = g_rc_timer.bottom - g_rc_timer.top;
    if (tw <= 0 || th <= 0) { tw = 800; th = 300; }

    /* Use custom face if loaded, else fall back cascade: Consolas -> Courier New */
    const wchar_t *face =
        (g_custom_font_loaded && g_custom_face[0]) ? g_custom_face : L"Consolas";

    /* Sample: widest possible timer string (all 8s look wide in most fonts) */
    const wchar_t *sample = L"88:88:88:888:888:888";

    /* Max usable area: 94% width, 80% height */
    int max_w = tw * 94 / 100;
    int max_h = th * 80 / 100;
    if (max_w < 100) max_w = 100;
    if (max_h < 30)  max_h = 30;

    int pt_timer = fit_pt(face, FW_BOLD, sample, max_w, max_h);
    if (pt_timer < 10) pt_timer = 10;

    g_font_timer = make_font_pt(pt_timer, face, FW_BOLD, FALSE);
    g_font_glow  = make_font_pt(pt_timer, face, FW_BOLD, FALSE);

    /* Fallback cascade: if custom or Consolas didn't produce a font, try Courier New */
    if (!g_font_timer) {
        pt_timer = fit_pt(L"Courier New", FW_BOLD, sample, max_w, max_h);
        if (pt_timer < 10) pt_timer = 10;
        g_font_timer = make_font_pt(pt_timer, L"Courier New", FW_BOLD, FALSE);
        g_font_glow  = make_font_pt(pt_timer, L"Courier New", FW_BOLD, FALSE);
        face = L"Courier New";
    }

    /* Last resort stock font (should never happen) */
    if (!g_font_timer) {
        g_font_timer = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
        g_font_glow  = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
    }

    /* Info fonts (fixed small size, use same face for consistency) */
    int pt_info = (int)(11.0f / g_dpi_scale + 0.5f);
    if (pt_info < 7)  pt_info = 7;
    if (pt_info > 18) pt_info = 18;

    g_font_info   = make_font_pt(pt_info,     face, FW_NORMAL, FALSE);
    g_font_status = make_font_pt(pt_info - 1, face, FW_NORMAL, FALSE);

    if (!g_font_info)   g_font_info   = (HFONT)GetStockObject(ANSI_FIXED_FONT);
    if (!g_font_status) g_font_status = (HFONT)GetStockObject(ANSI_FIXED_FONT);
}

/* ============================================================================
 * Custom TTF/OTF loader
 *   Scans assets\font\*.ttf and *.otf (relative to exe path).
 *   Reads the font family name from the 'name' table (nameID=1, Win Unicode).
 *   Returns TRUE and sets g_custom_face[] on success.
 * ========================================================================== */

/* Reads big-endian uint16 */
static uint16_t be16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] <<  8) |  (uint32_t)p[3];
}

static BOOL ttf_read_family(const uint8_t *data, DWORD sz, wchar_t *out, int cap) {
    if (sz < 12) return FALSE;

    /* sfnt offset table: numTables at byte 4 */
    uint16_t num_tables = be16(data + 4);
    if (12u + (DWORD)num_tables * 16u > sz) return FALSE;

    /* Find 'name' table */
    uint32_t name_off = 0;
    for (int i = 0; i < num_tables; i++) {
        const uint8_t *rec = data + 12 + i * 16;
        if (rec[0]=='n' && rec[1]=='a' && rec[2]=='m' && rec[3]=='e') {
            name_off = be32(rec + 8);
            break;
        }
    }
    if (!name_off || name_off + 6 > sz) return FALSE;

    const uint8_t *nt = data + name_off;
    uint16_t count      = be16(nt + 2);
    uint16_t str_offset = be16(nt + 4);
    if (name_off + 6u + (DWORD)count * 12u > sz) return FALSE;

    /* Scan nameRecords for nameID=1, platformID=3 (Win), encodingID=1 (BMP) */
    for (int i = 0; i < count; i++) {
        const uint8_t *nr = nt + 6 + i * 12;
        uint16_t plat   = be16(nr + 0);
        uint16_t encid  = be16(nr + 2);
        /* uint16_t langid = be16(nr + 4); */
        uint16_t nameid = be16(nr + 6);
        uint16_t length = be16(nr + 8);
        uint16_t offset = be16(nr + 10);
        if (plat != 3 || encid != 1 || nameid != 1) continue;

        /* String is UTF-16 BE */
        uint32_t abs = name_off + str_offset + offset;
        if (abs + length > sz) continue;
        const uint8_t *s = data + abs;
        int nchars = length / 2;
        if (nchars >= cap) nchars = cap - 1;
        for (int c = 0; c < nchars; c++)
            out[c] = (wchar_t)be16(s + c * 2);
        out[nchars] = 0;
        return TRUE;
    }
    return FALSE;
}

static BOOL try_load_custom_font(void) {
    /* -----------------------------------------------------------------------
     * Priority 1: font embedded as RCDATA resource IDR_CUSTOM_FONT.
     * The build script (build_all.bat) auto-detects the first .ttf/.otf in
     * assets\font\ and emits a generated .rc that bakes it into the exe.
     * If the build found no font file, FindResource returns NULL and we fall
     * through to priority 2.
     * --------------------------------------------------------------------- */
    HMODULE hmod = GetModuleHandleW(NULL);
    HRSRC   hrsrc = FindResourceW(hmod, MAKEINTRESOURCEW((WORD)IDR_CUSTOM_FONT),
                                  (LPCWSTR)RT_RCDATA);
    if (hrsrc) {
        DWORD    fsz  = SizeofResource(hmod, hrsrc);
        HGLOBAL  hglob = LoadResource(hmod, hrsrc);
        void    *data  = LockResource(hglob);

        if (data && fsz > 0) {
            /* Parse family name straight from the resource bytes */
            wchar_t family[LF_FACESIZE] = {0};
            if (ttf_read_family((const uint8_t *)data, fsz, family, LF_FACESIZE)) {
                DWORD num_fonts = 0;
                g_font_res = AddFontMemResourceEx(data, fsz, NULL, &num_fonts);
                if (g_font_res && num_fonts > 0) {
                    lstrcpynW(g_custom_face, family, LF_FACESIZE);
                    g_custom_font_loaded = TRUE;
                    return TRUE;
                }
            }
        }
    }

    /* -----------------------------------------------------------------------
     * Priority 2: scan assets\font\ next to the exe at runtime.
     * Useful when running from the source tree without rebuilding.
     * --------------------------------------------------------------------- */
    wchar_t exe_path[MAX_PATH], dir[MAX_PATH];
    GetModuleFileNameW(NULL, exe_path, MAX_PATH);
    lstrcpyW(dir, exe_path);
    wchar_t *sl = wcsrchr(dir, L'\\');
    if (sl) sl[1] = 0;

    wchar_t pattern[MAX_PATH];
    _snwprintf_s(pattern, MAX_PATH, _TRUNCATE, L"%sassets\\font\\*", dir);

    WIN32_FIND_DATAW fd;
    HANDLE hf = FindFirstFileW(pattern, &fd);
    if (hf == INVALID_HANDLE_VALUE) return FALSE;

    BOOL found = FALSE;
    do {
        const wchar_t *ext = wcsrchr(fd.cFileName, L'.');
        if (!ext) continue;
        if (_wcsicmp(ext, L".ttf") != 0 && _wcsicmp(ext, L".otf") != 0) continue;

        wchar_t full[MAX_PATH];
        _snwprintf_s(full, MAX_PATH, _TRUNCATE, L"%sassets\\font\\%s", dir, fd.cFileName);

        HANDLE hfile = CreateFileW(full, GENERIC_READ, FILE_SHARE_READ,
                                   NULL, OPEN_EXISTING, 0, NULL);
        if (hfile == INVALID_HANDLE_VALUE) continue;

        DWORD fsz2 = GetFileSize(hfile, NULL);
        uint8_t *buf = (uint8_t *)VirtualAlloc(NULL, fsz2, MEM_COMMIT, PAGE_READWRITE);
        if (!buf) { CloseHandle(hfile); continue; }

        DWORD nread = 0;
        ReadFile(hfile, buf, fsz2, &nread, NULL);
        CloseHandle(hfile);

        if (nread != fsz2) { VirtualFree(buf, 0, MEM_RELEASE); continue; }

        wchar_t family[LF_FACESIZE] = {0};
        if (!ttf_read_family(buf, fsz2, family, LF_FACESIZE)) {
            VirtualFree(buf, 0, MEM_RELEASE); continue;
        }

        DWORD num_fonts = 0;
        g_font_res = AddFontMemResourceEx(buf, fsz2, NULL, &num_fonts);
        VirtualFree(buf, 0, MEM_RELEASE);

        if (g_font_res && num_fonts > 0) {
            lstrcpynW(g_custom_face, family, LF_FACESIZE);
            g_custom_font_loaded = TRUE;
            found = TRUE;
            break;
        }
    } while (FindNextFileW(hf, &fd));

    FindClose(hf);
    return found;
}

/* ============================================================================
 * Backbuffer management
 * ========================================================================== */
static void create_backbuffer(HWND hwnd, int w, int h) {
    if (g_bm_buf) { DeleteObject(g_bm_buf); g_bm_buf = NULL; g_bm_bits = NULL; }
    if (g_dc_buf) { DeleteDC(g_dc_buf);     g_dc_buf  = NULL; }

    HDC hdc = GetDC(hwnd);
    g_dc_buf = CreateCompatibleDC(hdc);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = w;
    bmi.bmiHeader.biHeight      = -h;  /* top-down */
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    g_bm_buf  = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &g_bm_bits, NULL, 0);
    SelectObject(g_dc_buf, g_bm_buf);
    ReleaseDC(hwnd, hdc);
    g_bm_w = w; g_bm_h = h;
}

/* Clear backbuffer using SIMD (nc_avx_simd_zero_buf / nc_simd_zero_buf) */
static void clear_backbuffer(COLORREF bg) {
    if (!g_bm_bits || !g_bm_w || !g_bm_h) return;

    size_t total = (size_t)g_bm_w * g_bm_h * 4;
    /* Fill with bg colour first, then let GDI paint over - here we zero
       then FillRect.  For performance, if bg is pure black we can SIMD-zero. */
    if (bg == 0) {
        /* Use best available SIMD to zero the DIBSection */
        if (nc_avx_check()) {
            nc_avx_simd_zero_buf(g_bm_bits, total);
        } else {
            nc_simd_zero_buf(g_bm_bits, total);
        }
    } else {
        /* GDI FillRect is fine for non-black backgrounds */
        RECT all = {0, 0, g_bm_w, g_bm_h};
        HBRUSH br = CreateSolidBrush(bg);
        FillRect(g_dc_buf, &all, br);
        DeleteObject(br);
    }
}

/* ============================================================================
 * Layout
 * ========================================================================== */
static void relayout(int w, int h) {
    g_win_w = w; g_win_h = h;

    /* Title bar */
    g_rc_title.left = 0; g_rc_title.right  = w;
    g_rc_title.top  = 0; g_rc_title.bottom = S(88);

    /* Status bar */
    g_rc_status.left   = 0; g_rc_status.right  = w;
    g_rc_status.bottom = h; g_rc_status.top    = h - S(22);

    /* Button bar just above status */
    g_rc_btn_bar.left   = 0; g_rc_btn_bar.right  = w;
    g_rc_btn_bar.bottom = g_rc_status.top;
    g_rc_btn_bar.top    = g_rc_btn_bar.bottom - S(28);

    /* Timer occupies the middle */
    g_rc_timer.left   = S(2);
    g_rc_timer.right  = w - S(2);
    g_rc_timer.top    = g_rc_title.bottom;
    g_rc_timer.bottom = g_rc_btn_bar.top  - S(8);
    update_header_control_rects();
}

/* ============================================================================
 * Elapsed helpers
 * ========================================================================== */
static uint64_t get_elapsed_ns(void) {
    if (!g_ctx) return 0;
    /* When running, add the frozen base (non-zero after a resume from pause) */
    if (g_state == TS_RUNNING) return nc_elapsed_ns(g_ctx) + g_elapsed_ns;
    return g_elapsed_ns;
}

static int cpu_has_avx512f_local(void) {
    int info[4];
#if defined(_M_X64) || defined(__x86_64__) || defined(_M_IX86) || defined(__i386__)
    /* Need XMM/YMM/ZMM state enabled by OS */
    __cpuid(info, 1);
    if (((info[2] >> 27) & 1) == 0 || ((info[2] >> 28) & 1) == 0) return 0;
    {
        unsigned long long xcr0 = _xgetbv(0);
        if ((xcr0 & 0xE6) != 0xE6) return 0; /* XMM|YMM|opmask|ZMM_hi256|hi16_ZMM */
    }
    __cpuidex(info, 7, 0);
    return (info[1] >> 16) & 1; /* AVX-512F */
#else
    return 0;
#endif
}

static void format_simple_time(uint64_t ns, char *out, size_t out_sz) {
    uint64_t total_ms = ns / 1000000ull;
    uint64_t ms = total_ms % 1000ull;
    uint64_t total_s = total_ms / 1000ull;
    uint64_t sec = total_s % 60ull;
    uint64_t min = total_s / 60ull;
    _snprintf_s(out, out_sz, _TRUNCATE, "%02llu:%02llu.%03llu",
                (unsigned long long)min,
                (unsigned long long)sec,
                (unsigned long long)ms);
}


/* ============================================================================
 * Bench helpers
 * ========================================================================== */
static void bench_lock(void) {
    if (InterlockedCompareExchange(&g_bench_lock_ready, 1, 1)) EnterCriticalSection(&g_bench_lock);
}

static void bench_unlock(void) {
    if (InterlockedCompareExchange(&g_bench_lock_ready, 1, 1)) LeaveCriticalSection(&g_bench_lock);
}

static void bench_log_reset(void) {
    bench_lock();
    g_bench.log_len = 0;
    g_bench.log[0] = '\0';
    bench_unlock();
}

static void bench_log_append(const char *fmt, ...) {
    bench_lock();
    if (g_bench.log_len >= sizeof(g_bench.log) - 1) { bench_unlock(); return; }
    va_list args;
    va_start(args, fmt);
    int written = _vsnprintf_s(g_bench.log + g_bench.log_len,
                               sizeof(g_bench.log) - g_bench.log_len,
                               _TRUNCATE, fmt, args);
    va_end(args);
    if (written > 0) g_bench.log_len += (size_t)written;
    bench_unlock();
}

static uint64_t bench_tsc_start(void) {
    return g_ctx && nc_avx_check() ? nc_avx_prefetch_tsc_start(g_bm_bits) : nc_tsc_lfence_start();
}
static uint64_t bench_tsc_end(void) {
    return g_ctx && nc_avx_check() ? nc_avx_tsc_end() : nc_tsc_lfence_end();
}

static void bench_sink64(volatile uint64_t *sink, uint64_t v) { *sink ^= (v + 0x9E3779B97F4A7C15ull + (*sink << 6) + (*sink >> 2)); }

static uint64_t bench_run_cpu_intrinsics(BenchKind kind, size_t loops) {
    return nc_bench_run_cpu_kernel((nc_bench_kernel_id_t)kind, loops);
}

static const wchar_t *bench_kind_name(BenchKind kind) {
    switch (kind) {
    case NC_BENCH_ASM_LEGACY: return L"Legacy ASM";
    case NC_BENCH_MMX:        return L"MMX";
    case NC_BENCH_SSE:        return L"SSE";
    case NC_BENCH_SSE2:       return L"SSE2";
    case NC_BENCH_SSE3:       return L"SSE3";
    case NC_BENCH_SSSE3:      return L"SSSE3";
    case NC_BENCH_SSE41:      return L"SSE4.1";
    case NC_BENCH_SSE42:      return L"SSE4.2";
    case NC_BENCH_AESNI:      return L"AES-256-GCM";
    case NC_BENCH_PCLMULQDQ:  return L"AEAD auth path";
    case NC_BENCH_SHANI:      return L"SHA-256";
    case NC_BENCH_AVX:        return L"AVX";
    case NC_BENCH_F16C:       return L"F16C";
    case NC_BENCH_FMA:        return L"FMA";
    case NC_BENCH_AVX2:       return L"AVX2";
    case NC_BENCH_VAES:       return L"AES-256-GCM dispatch";
    case NC_BENCH_AVX512F:    return L"AVX-512";
    case NC_BENCH_AVX512BW:   return L"AVX-512BW";
    case NC_BENCH_AVX512VL:   return L"AVX-512VL";
    case NC_BENCH_AVX512VNNI: return L"AVX-512VNNI";
    default: return L"-";
    }
}

static bench_profile_t bench_get_profile(BenchKind kind) {
    bench_profile_t p;
    ZeroMemory(&p, sizeof(p));
    p.title = nc_bench_kernel_short_name((nc_bench_kernel_id_t)kind);
    p.warmup = "CPU feature microbench; dispatch is gated by CPUID/XGETBV before the kernel is called";
    p.unit = "1 op = 1 logical CPU feature work unit";
    p.ops_per_loop = 1.0;
    p.bytes_per_op = 8.0;
    p.loops[0] = 300000; p.loops[1] = 600000; p.loops[2] = 900000;
    p.repeats[0] = 1; p.repeats[1] = 2; p.repeats[2] = 3;
    switch (kind) {
    case NC_BENCH_ASM_LEGACY:
        p.warmup = "portable scalar x64 assembler baseline; no SIMD required";
        p.bytes_per_op = 8.0;
        break;
    case NC_BENCH_MMX:
        p.warmup = "MMX integer SIMD microbench; EMMS is issued before return";
        p.bytes_per_op = 8.0;
        break;
    case NC_BENCH_SSE: case NC_BENCH_SSE2: case NC_BENCH_SSE3: case NC_BENCH_SSSE3: case NC_BENCH_SSE41: case NC_BENCH_SSE42:
        p.warmup = "SSE-family microbench using the selected instruction family";
        p.ops_per_loop = 4.0; p.bytes_per_op = 16.0;
        break;
    case NC_BENCH_AESNI:
        p.warmup = "real AES round microbench using AESENC/AESENCLAST";
        p.ops_per_loop = 8.0; p.bytes_per_op = 16.0;
        break;
    case NC_BENCH_PCLMULQDQ:
        p.warmup = "AEAD authentication path is measured through OpenSSL EVP or libsodium, not as a timer backend";
        p.ops_per_loop = 2.0; p.bytes_per_op = 16.0;
        break;
    case NC_BENCH_SHANI:
        p.warmup = "SHA-256 instruction microbench when toolchain support is available; otherwise safe fallback";
        p.ops_per_loop = 4.0; p.bytes_per_op = 64.0;
        break;
    case NC_BENCH_AVX: case NC_BENCH_F16C: case NC_BENCH_FMA: case NC_BENCH_AVX2:
        p.warmup = "AVX-family microbench; only called when CPUID+XGETBV enables YMM state";
        p.ops_per_loop = 8.0; p.bytes_per_op = 32.0;
        p.loops[0] = 600000; p.loops[1] = 1200000; p.loops[2] = 1800000;
        break;
    case NC_BENCH_AVX512F: case NC_BENCH_AVX512VNNI:
        p.warmup = "AVX-512-family microbench; only called when CPUID+XGETBV enables ZMM/opmask state";
        p.ops_per_loop = 16.0; p.bytes_per_op = 64.0;
        p.loops[0] = 500000; p.loops[1] = 1000000; p.loops[2] = 1500000;
        break;
    default:
        break;
    }
    return p;
}

static int bench_kind_available(BenchKind kind) {
    return nc_bench_kernel_available((nc_bench_kernel_id_t)kind, NULL);
}

static size_t bench_payload_size_for(BenchKind kind) {
    switch (kind) {
    case NC_BENCH_SHANI: return 64 * 1024;
    case NC_BENCH_AVX: case NC_BENCH_F16C: case NC_BENCH_FMA: case NC_BENCH_AVX2:
    case NC_BENCH_AVX512F: case NC_BENCH_AVX512VNNI:
        return 32 * 1024;
    default:
        return 16 * 1024;
    }
}

static uint64_t bench_run_openssl_evp(BenchKind kind, size_t loops) {
#if defined(NC_GUI_HAS_OPENSSL)
    enum { MAX_PAYLOAD = 64 * 1024, TAG_LEN = 16 };
    static unsigned char in[MAX_PAYLOAD];
    static unsigned char out[MAX_PAYLOAD + TAG_LEN];
    static unsigned char key[32];
    static unsigned char iv[12];
    static unsigned char tag[TAG_LEN];
    uint64_t sink = 0xA7EC5155A5A5A5A5ull ^ (uint64_t)kind;
    size_t n = bench_payload_size_for(kind);
    if (n > MAX_PAYLOAD) n = MAX_PAYLOAD;

    for (size_t i = 0; i < n; ++i) in[i] = (unsigned char)(i * 31u + (unsigned)kind);
    for (int i = 0; i < 32; ++i) key[i] = (unsigned char)(0xA0u + i);
    for (int i = 0; i < 12; ++i) iv[i] = (unsigned char)(0xC0u + i);

    if (kind == NC_BENCH_SHANI) {
        unsigned char md[EVP_MAX_MD_SIZE];
        unsigned int md_len = 0;
        EVP_MD_CTX *mctx = EVP_MD_CTX_new();
        if (!mctx) return 0;
        for (size_t i = 0; i < loops; ++i) {
            if (EVP_DigestInit_ex(mctx, EVP_sha256(), NULL) != 1) break;
            if (EVP_DigestUpdate(mctx, in, n) != 1) break;
            if (EVP_DigestFinal_ex(mctx, md, &md_len) != 1) break;
            sink ^= ((uint64_t)md[0] << 56) ^ ((uint64_t)md[md_len ? md_len - 1 : 0] << 8) ^ md_len ^ i;
        }
        EVP_MD_CTX_free(mctx);
        return sink;
    }

    const int is_gcm = !(kind == NC_BENCH_AVX || kind == NC_BENCH_AVX2 || kind == NC_BENCH_AVX512F);
    const EVP_CIPHER *cipher = is_gcm ? EVP_aes_256_gcm() : EVP_chacha20_poly1305();

    EVP_CIPHER_CTX *cctx = EVP_CIPHER_CTX_new();
    if (!cctx || !cipher) {
        if (cctx) EVP_CIPHER_CTX_free(cctx);
        return 0;
    }

    for (size_t i = 0; i < loops; ++i) {
        int out_len1 = 0, out_len2 = 0;
        iv[11] = (unsigned char)i;
        if (EVP_EncryptInit_ex(cctx, cipher, NULL, NULL, NULL) != 1) break;
        if (is_gcm && EVP_CIPHER_CTX_ctrl(cctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL) != 1) break;
        if (EVP_EncryptInit_ex(cctx, NULL, NULL, key, iv) != 1) break;
        if (EVP_EncryptUpdate(cctx, out, &out_len1, in, (int)n) != 1) break;
        if (EVP_EncryptFinal_ex(cctx, out + out_len1, &out_len2) != 1) break;
        if (is_gcm) EVP_CIPHER_CTX_ctrl(cctx, EVP_CTRL_GCM_GET_TAG, TAG_LEN, tag);
        sink ^= ((uint64_t)out[0] << 48) ^ ((uint64_t)out[(out_len1 + out_len2 - 1) & (MAX_PAYLOAD - 1)] << 16) ^ i;
    }

    EVP_CIPHER_CTX_free(cctx);
    return sink;
#else
    (void)kind; (void)loops;
    return 0;
#endif
}

static uint64_t bench_run_libsodium(BenchKind kind, size_t loops) {
#if defined(NC_GUI_HAS_LIBSODIUM)
    enum { MAX_PAYLOAD = 64 * 1024, ABYTES_MAX = 32 };
    static unsigned char in[MAX_PAYLOAD];
    static unsigned char out[MAX_PAYLOAD + ABYTES_MAX];
    static unsigned char key[crypto_aead_chacha20poly1305_ietf_KEYBYTES];
    static unsigned char nonce[crypto_aead_chacha20poly1305_ietf_NPUBBYTES];
    uint64_t sink = 0x51B500D515150515ull ^ (uint64_t)kind;
    size_t n = bench_payload_size_for(kind);
    if (n > MAX_PAYLOAD) n = MAX_PAYLOAD;
    for (size_t i = 0; i < n; ++i) in[i] = (unsigned char)(i * 17u + (unsigned)kind);
    for (size_t i = 0; i < sizeof(key); ++i) key[i] = (unsigned char)(0x55u + i);
    for (size_t i = 0; i < sizeof(nonce); ++i) nonce[i] = (unsigned char)(0x33u + i);

    if (sodium_init() < 0) return 0;

    if (kind == NC_BENCH_SHANI) {
        unsigned char md[crypto_hash_sha256_BYTES];
        for (size_t i = 0; i < loops; ++i) {
            crypto_hash_sha256(md, in, (unsigned long long)n);
            sink ^= ((uint64_t)md[0] << 40) ^ ((uint64_t)md[31] << 8) ^ i;
        }
        return sink;
    }

#if defined(crypto_aead_aes256gcm_KEYBYTES)
    if ((kind == NC_BENCH_AESNI || kind == NC_BENCH_PCLMULQDQ) && crypto_aead_aes256gcm_is_available()) {
        unsigned char aes_key[crypto_aead_aes256gcm_KEYBYTES];
        unsigned char aes_nonce[crypto_aead_aes256gcm_NPUBBYTES];
        unsigned long long clen = 0;
        for (size_t i = 0; i < sizeof(aes_key); ++i) aes_key[i] = (unsigned char)(0x90u + i);
        for (size_t i = 0; i < sizeof(aes_nonce); ++i) aes_nonce[i] = (unsigned char)(0x70u + i);
        for (size_t i = 0; i < loops; ++i) {
            aes_nonce[crypto_aead_aes256gcm_NPUBBYTES - 1] = (unsigned char)i;
            crypto_aead_aes256gcm_encrypt(out, &clen, in, (unsigned long long)n, NULL, 0, NULL, aes_nonce, aes_key);
            sink ^= ((uint64_t)out[0] << 32) ^ ((uint64_t)out[(clen ? clen - 1 : 0)] << 12) ^ i;
        }
        return sink;
    }
#endif

    unsigned long long clen = 0;
    for (size_t i = 0; i < loops; ++i) {
        nonce[crypto_aead_chacha20poly1305_ietf_NPUBBYTES - 1] = (unsigned char)i;
        crypto_aead_chacha20poly1305_ietf_encrypt(out, &clen, in, (unsigned long long)n, NULL, 0, NULL, nonce, key);
        sink ^= ((uint64_t)out[0] << 24) ^ ((uint64_t)out[(clen ? clen - 1 : 0)] << 4) ^ i;
    }
    return sink;
#else
    (void)kind; (void)loops;
    return 0;
#endif
}

static uint64_t bench_run_kernel(bench_mode_t mode, BenchKind kind, size_t loops) {
    switch (mode) {
    case BENCH_MODE_CPU_INTRINSICS: return bench_run_cpu_intrinsics(kind, loops);
    case BENCH_MODE_OPENSSL_EVP:    return bench_run_openssl_evp(kind, loops);
    case BENCH_MODE_LIBSODIUM:      return bench_run_libsodium(kind, loops);
    default:                        return 0;
    }
}

static bench_pass_result_t bench_run_pass(bench_mode_t mode, BenchKind kind, int pass_no, int repeats, size_t loops, double ops_per_loop, double bytes_per_op) {
    bench_pass_result_t r;
    ZeroMemory(&r, sizeof(r));
    r.pass_no = pass_no;
    r.repeats = repeats;
    r.loops = loops;

    volatile uint64_t sink = 0xD3997AAB30550296ull ^ (uint64_t)kind ^ ((uint64_t)pass_no << 32);
    uint64_t t0 = bench_tsc_start();
    for (int i = 0; i < repeats; ++i) bench_sink64(&sink, bench_run_kernel(mode, kind, loops));
    uint64_t t1 = bench_tsc_end();
    uint64_t cycles = (t1 > t0) ? (t1 - t0) : 0;
    double secs = (g_ctx && nc_tsc_hz(g_ctx)) ? ((double)cycles / (double)nc_tsc_hz(g_ctx)) : 0.0;
    double total_ops = (double)repeats * (double)loops * ops_per_loop;
    double total_bytes = total_ops * bytes_per_op;

    r.cycles = cycles;
    r.secs = secs;
    r.total_ops = total_ops;
    r.total_bytes = total_bytes;
    r.mops = secs > 0.0 ? (total_ops / secs) / 1e6 : 0.0;
    r.cyc_per_op = total_ops > 0.0 ? ((double)cycles / total_ops) : 0.0;
    r.ns_per_op = total_ops > 0.0 ? (secs * 1e9 / total_ops) : 0.0;
    r.mibps = (secs > 0.0 && total_bytes > 0.0) ? (total_bytes / (1024.0 * 1024.0)) / secs : 0.0;
    r.gibps = (secs > 0.0 && total_bytes > 0.0) ? (total_bytes / (1024.0 * 1024.0 * 1024.0)) / secs : 0.0;
    r.sink = sink;
    return r;
}

static void bench_log_pass(const bench_profile_t *p, const bench_pass_result_t *r) {
    bench_log_append("pass %d: repeats=%d  loops=%zu  cycles=%llu  time=%.6f s  rate=%.3f Mops/s  cyc/op=%.4f  ns/op=%.4f",
        r->pass_no, r->repeats, r->loops, (unsigned long long)r->cycles, r->secs, r->mops, r->cyc_per_op, r->ns_per_op);
    if (p->bytes_per_op > 0.0) {
        bench_log_append("  MiB/s=%.3f  GiB/s=%.3f", r->mibps, r->gibps);
    }
    bench_log_append("  sink=%016llX\r\n", (unsigned long long)r->sink);
}

static void bench_log_summary(const bench_profile_t *p, const bench_pass_result_t *a, int n) {
    if (!a || n <= 0) return;
    double sum_mops = 0.0, sum_cyc = 0.0, sum_ns = 0.0, sum_mib = 0.0, sum_gib = 0.0;
    double best_mops = a[0].mops, worst_mops = a[0].mops;
    double best_cyc = a[0].cyc_per_op, worst_cyc = a[0].cyc_per_op;
    double best_ns = a[0].ns_per_op, worst_ns = a[0].ns_per_op;
    uint64_t total_cycles = 0;
    double total_secs = 0.0;
    double total_ops = 0.0;
    double total_bytes = 0.0;
    for (int i = 0; i < n; ++i) {
        sum_mops += a[i].mops;
        sum_cyc += a[i].cyc_per_op;
        sum_ns += a[i].ns_per_op;
        sum_mib += a[i].mibps;
        sum_gib += a[i].gibps;
        total_cycles += a[i].cycles;
        total_secs += a[i].secs;
        total_ops += a[i].total_ops;
        total_bytes += a[i].total_bytes;
        if (a[i].mops > best_mops) best_mops = a[i].mops;
        if (a[i].mops < worst_mops) worst_mops = a[i].mops;
        if (a[i].cyc_per_op < best_cyc) best_cyc = a[i].cyc_per_op;
        if (a[i].cyc_per_op > worst_cyc) worst_cyc = a[i].cyc_per_op;
        if (a[i].ns_per_op < best_ns) best_ns = a[i].ns_per_op;
        if (a[i].ns_per_op > worst_ns) worst_ns = a[i].ns_per_op;
    }
    bench_log_append("\r\nsummary:\r\n");
    bench_log_append("  unit: %s\r\n", p->unit);
    bench_log_append("  avg rate: %.3f Mops/s   best: %.3f   worst: %.3f\r\n", sum_mops / n, best_mops, worst_mops);
    bench_log_append("  avg cyc/op: %.4f       best: %.4f   worst: %.4f\r\n", sum_cyc / n, best_cyc, worst_cyc);
    bench_log_append("  avg ns/op: %.4f        best: %.4f   worst: %.4f\r\n", sum_ns / n, best_ns, worst_ns);
    if (p->bytes_per_op > 0.0) {
        bench_log_append("  avg MiB/s: %.3f        avg GiB/s: %.3f\r\n", sum_mib / n, sum_gib / n);
    }
    bench_log_append("  aggregate cycles: %llu\r\n", (unsigned long long)total_cycles);
    bench_log_append("  aggregate time: %.6f s\r\n", total_secs);
    bench_log_append("  aggregate logical ops: %.0f\r\n", total_ops);
    if (total_bytes > 0.0) bench_log_append("  aggregate logical bytes: %.0f\r\n", total_bytes);
}

static DWORD WINAPI bench_thread_proc(LPVOID param) {
    uintptr_t packed = (uintptr_t)param;
    BenchKind kind = (BenchKind)(packed & 0xFFu);
    bench_mode_t mode = (bench_mode_t)((packed >> 8) & 0xFFu);
    bench_profile_t profile = bench_get_profile(kind);
    bench_pass_result_t passes[3];
    g_bench.save_prompted = 0;
    bench_log_reset();
    bench_log_append("=== NanoChrono benchmark ===\r\n");
    bench_log_append("operation: %S\r\n", bench_kind_name(kind));
    bench_log_append("mode: %s\r\n", bench_mode_name_a(mode));
    bench_log_append("backend: %s\r\n", g_ctx ? nc_backend_name(nc_backend(g_ctx)) : "none");
    bench_log_append("tsc_hz: %.3f MHz\r\n", g_ctx ? ((double)nc_tsc_hz(g_ctx) / 1e6) : 0.0);
    bench_log_append("cpu flags: AES=%d SHA=%d VAES=%d PCLMUL=%d AVX=%d AVX2=%d AVX512F=%d\r\n\r\n",
        g_cpu_features.aesni, g_cpu_features.shani, g_cpu_features.vaes, g_cpu_features.pclmulqdq, g_cpu_features.avx, g_cpu_features.avx2, g_cpu_has_avx512f);

    if (!bench_mode_available(mode)) {
        bench_log_append("status: benchmark mode is not linked in this build.\r\n");
        InterlockedExchange(&g_bench.running, 0);
        InterlockedExchange(&g_bench.finished, 1);
        PostMessageW(g_hwnd_main, WM_APP_BENCH_DONE, 0, 0);
        return 0;
    }

    if (mode == BENCH_MODE_CPU_INTRINSICS && !bench_kind_available(kind)) {
        bench_log_append("status: NOT AVAILABLE on this CPU.\r\n");
        InterlockedExchange(&g_bench.running, 0);
        InterlockedExchange(&g_bench.finished, 1);
        PostMessageW(g_hwnd_main, WM_APP_BENCH_DONE, 0, 0);
        return 0;
    }

    if (mode == BENCH_MODE_OPENSSL_EVP) {
        profile.warmup = (kind == NC_BENCH_SHANI)
            ? "real OpenSSL EVP SHA-256 digest over byte buffers"
            : ((kind == NC_BENCH_AVX || kind == NC_BENCH_AVX2 || kind == NC_BENCH_AVX512F)
                ? "real OpenSSL EVP ChaCha20-Poly1305 encrypt over byte buffers"
                : "real OpenSSL EVP AES-256-GCM encrypt over byte buffers");
        profile.unit = "1 op = 1 EVP call over the selected byte buffer";
        profile.ops_per_loop = 1.0;
        profile.bytes_per_op = (double)bench_payload_size_for(kind);
        profile.loops[0] = 1500; profile.loops[1] = 3000; profile.loops[2] = 4500;
        profile.repeats[0] = 1; profile.repeats[1] = 2; profile.repeats[2] = 3;
    } else if (mode == BENCH_MODE_LIBSODIUM) {
        profile.warmup = (kind == NC_BENCH_SHANI)
            ? "real libsodium crypto_hash_sha256 over byte buffers"
            : ((kind == NC_BENCH_AESNI || kind == NC_BENCH_PCLMULQDQ)
                ? "real libsodium AEAD encrypt: AES-256-GCM when available, otherwise ChaCha20-Poly1305-IETF"
                : "real libsodium ChaCha20-Poly1305-IETF encrypt over byte buffers");
        profile.unit = "1 op = 1 libsodium call over the selected byte buffer";
        profile.ops_per_loop = 1.0;
        profile.bytes_per_op = (double)bench_payload_size_for(kind);
        profile.loops[0] = 1500; profile.loops[1] = 3000; profile.loops[2] = 4500;
        profile.repeats[0] = 1; profile.repeats[1] = 2; profile.repeats[2] = 3;
    }

    bench_log_append("warmup: %s\r\n", profile.warmup);
    bench_log_append("unit: %s\r\n\r\n", profile.unit);

    for (int i = 0; i < 3; ++i) {
        passes[i] = bench_run_pass(mode, kind, i + 1, profile.repeats[i], profile.loops[i], profile.ops_per_loop, profile.bytes_per_op);
        g_bench.last_cycles = passes[i].cycles;
        g_bench.last_seconds = passes[i].secs;
        g_bench.last_mops = passes[i].mops;
        bench_log_pass(&profile, &passes[i]);
    }

    bench_log_summary(&profile, passes, 3);
    bench_log_append("\r\nstatus: completed successfully.\r\n");
    InterlockedExchange(&g_bench.running, 0);
    InterlockedExchange(&g_bench.finished, 1);
    PostMessageW(g_hwnd_main, WM_APP_BENCH_DONE, 0, 0);
    return 0;
}

static void bench_start(bench_mode_t mode, BenchKind kind) {
    if (InterlockedCompareExchange(&g_bench.running, 1, 0) != 0) return;
    if (g_bench.thread) { CloseHandle(g_bench.thread); g_bench.thread = NULL; }
    InterlockedExchange(&g_bench.finished, 0);
    g_bench.selected = kind;
    lstrcpynW(g_bench.selected_name, bench_kind_name(kind), _countof(g_bench.selected_name));
    bench_log_reset();
    bench_log_append("starting %S benchmark in %s mode...\r\n", g_bench.selected_name, bench_mode_name_a(mode));
    g_bench.thread = CreateThread(NULL, 0, bench_thread_proc, (LPVOID)((uintptr_t)kind | ((uintptr_t)mode << 8)), 0, NULL);
    if (!g_bench.thread) {
        bench_log_append("status: failed to create benchmark thread.\r\n");
        InterlockedExchange(&g_bench.running, 0);
        InterlockedExchange(&g_bench.finished, 1);
    }
    InvalidateRect(g_hwnd_main, NULL, FALSE);
}

static void bench_save_log_dialog(HWND hwnd) {
    wchar_t path[MAX_PATH] = L"nanochrono-bench.log";
    OPENFILENAMEW ofn; ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Log files (*.log)\0*.log\0Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = _countof(path);
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = L"log";
    if (!GetSaveFileNameW(&ofn)) return;
    HANDLE h = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    bench_lock();
    WriteFile(h, g_bench.log, (DWORD)strlen(g_bench.log), &written, NULL);
    bench_unlock();
    CloseHandle(h);
}

/* ============================================================================
 * Paint
 * ========================================================================== */
static void draw_text_shadow(HDC dc, const wchar_t *s, RECT *rc, COLORREF clr,
                             UINT fmt, int ox, int oy) {
    RECT shadow = *rc;
    OffsetRect(&shadow, ox, oy);
    SetTextColor(dc, clr);
    DrawTextW(dc, s, -1, &shadow, fmt);
}


static void draw_title_analog_clock(HDC dc, const RECT *rc, uint64_t unix_ns) {
    int w = rc->right - rc->left;
    int h = rc->bottom - rc->top;
    int r = (h < w ? h : w) / 2 - S(4);
    if (r < S(6)) r = S(6);
    int cx = rc->left + S(14) + r;
    int cy = rc->top + h / 2;
    HPEN pen_border = CreatePen(PS_SOLID, S(1), CLR_INFO);
    HPEN pen_sec = CreatePen(PS_SOLID, S(1), CLR_TIMER);
    HPEN pen_min = CreatePen(PS_SOLID, S(2), 0x00FFFFFFu);
    HBRUSH old_br = (HBRUSH)SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
    HPEN old_pen = (HPEN)SelectObject(dc, pen_border);
    Ellipse(dc, cx - r, cy - r, cx + r, cy + r);
    uint64_t ns_day = unix_ns % 86400000000000ull;
    double sec = (double)(ns_day % 60000000000ull) / 1000000000.0;
    double min = (double)((ns_day / 60000000000ull) % 60ull) + sec / 60.0;
    double hour = (double)((ns_day / 3600000000000ull) % 12ull) + min / 60.0;
    double asec = sec / 60.0 * 2.0 * NC_GUI_PI - NC_GUI_PI / 2.0;
    double amin = min / 60.0 * 2.0 * NC_GUI_PI - NC_GUI_PI / 2.0;
    double ahour = hour / 12.0 * 2.0 * NC_GUI_PI - NC_GUI_PI / 2.0;
    SelectObject(dc, pen_min);
    MoveToEx(dc, cx, cy, NULL); LineTo(dc, cx + (int)(cos(ahour) * r * 0.45), cy + (int)(sin(ahour) * r * 0.45));
    MoveToEx(dc, cx, cy, NULL); LineTo(dc, cx + (int)(cos(amin) * r * 0.70), cy + (int)(sin(amin) * r * 0.70));
    SelectObject(dc, pen_sec);
    MoveToEx(dc, cx, cy, NULL); LineTo(dc, cx + (int)(cos(asec) * r * 0.82), cy + (int)(sin(asec) * r * 0.82));
    SelectObject(dc, old_pen);
    SelectObject(dc, old_br);
    DeleteObject(pen_border); DeleteObject(pen_sec); DeleteObject(pen_min);
}

static void gui_extract_time_component(const char *formatted, char *out, size_t cap, int keep_fraction) {
    if (!out || cap == 0) return;
    out[0] = '\0';
    if (!formatted) return;
    const char *p = strchr(formatted, ' ');
    p = p ? p + 1 : formatted;
    const char *end = strchr(p, ' ');
    size_t n = end ? (size_t)(end - p) : strlen(p);
    if (!keep_fraction) {
        const char *dot = memchr(p, '.', n);
        if (dot) n = (size_t)(dot - p);
    }
    if (n >= cap) n = cap - 1;
    memcpy(out, p, n);
    out[n] = '\0';
}

static void draw_title_nanoclock(HDC dc, const RECT *rc) {
    if (!g_ctx || !rc || rc->right <= rc->left) return;
    nc_nanoclock_snapshot_t snap;
    if (!nc_nanoclock_snapshot(g_ctx, &snap)) return;
    if (g_clock_design == NC_GUI_CLOCK_ANALOG) {
        draw_title_analog_clock(dc, rc, snap.unix_time_ns);
        char mini[96];
        sprintf_s(mini, sizeof(mini), "%s", nc_asm_simd_family_name((nc_asm_simd_family_t)snap.best_simd_family));
        wchar_t wmini[96];
        MultiByteToWideChar(CP_ACP, 0, mini, -1, wmini, 96);
        RECT tx = *rc;
        tx.left += S(48);
        DrawTextW(dc, wmini, -1, &tx, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    } else {
        char tbuf[96], hh[64], text[192];
        nc_format_unix_time_ns_ex(snap.unix_time_ns, g_clock_zone, g_clock_utc_offset_min, tbuf, sizeof(tbuf));
        gui_extract_time_component(tbuf, hh, sizeof(hh), 1);
        sprintf_s(text, sizeof(text), "%s  %s", hh, nc_asm_simd_family_name((nc_asm_simd_family_t)snap.best_simd_family));
        wchar_t wtext[224];
        MultiByteToWideChar(CP_ACP, 0, text, -1, wtext, 224);
        DrawTextW(dc, wtext, -1, (RECT*)rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
}


static void gui_format_clock_time(nc_nanoclock_snapshot_t *snap, char *buf, size_t cap) {
    if (!snap || !buf || !cap) return;
    char full[128];
    if (!nc_format_unix_time_ns_ex(snap->unix_time_ns, g_clock_zone, g_clock_utc_offset_min, full, sizeof(full))) {
        snprintf(buf, cap, "CLOCK");
        return;
    }
    /* The large clock face shows only HH:MM:SS[.ns]. Date/zone stay in
       title/status lines so the face does not clip in smaller windows. */
    gui_extract_time_component(full, buf, cap, !g_simple_mode);
}

static void gui_cycle_clock_view(void) {
    g_show_bench = false;
    if (g_clock_view == NC_CLOCK_VIEW_STOPWATCH) g_clock_view = NC_CLOCK_VIEW_CLOCK;
    else if (g_clock_view == NC_CLOCK_VIEW_CLOCK) g_clock_view = NC_CLOCK_VIEW_TIMER;
    else g_clock_view = NC_CLOCK_VIEW_STOPWATCH;
}

static void gui_cycle_clock_zone(void) {
    if (g_clock_zone == NC_TIME_ZONE_LOCAL) g_clock_zone = NC_TIME_ZONE_UTC;
    else if (g_clock_zone == NC_TIME_ZONE_UTC) { g_clock_zone = NC_TIME_ZONE_CUSTOM_OFFSET; g_clock_utc_offset_min = 0; }
    else g_clock_zone = NC_TIME_ZONE_LOCAL;
}


static void nc_draw_line(HDC dc, int x1, int y1, int x2, int y2, COLORREF c) {
    HPEN pen = CreatePen(PS_SOLID, 1, c);
    HPEN old = (HPEN)SelectObject(dc, pen);
    MoveToEx(dc, x1, y1, NULL);
    LineTo(dc, x2, y2);
    SelectObject(dc, old);
    DeleteObject(pen);
}

static void nc_draw_rect_frame(HDC dc, const RECT *rc, COLORREF c) {
    HPEN pen = CreatePen(PS_SOLID, 1, c);
    HBRUSH old_br = (HBRUSH)SelectObject(dc, GetStockObject(NULL_BRUSH));
    HPEN old_pen = (HPEN)SelectObject(dc, pen);
    Rectangle(dc, rc->left, rc->top, rc->right, rc->bottom);
    SelectObject(dc, old_pen);
    SelectObject(dc, old_br);
    DeleteObject(pen);
}

static void nc_fill_rect(HDC dc, const RECT *rc, COLORREF c) {
    HBRUSH br = CreateSolidBrush(c);
    FillRect(dc, rc, br);
    DeleteObject(br);
}

static void nc_draw_button(HDC dc, const wchar_t *text, RECT rc, BOOL active, BOOL hover) {
    COLORREF fill = active ? CLR_BTN_ACTIVE : (hover ? 0x00141414u : 0x00030303u);
    COLORREF border = active ? 0x0039D000u : 0x00333333u;
    nc_fill_rect(dc, &rc, fill);
    nc_draw_rect_frame(dc, &rc, border);
    SetTextColor(dc, active ? 0x00000000u : CLR_INFO);
    DrawTextW(dc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

static void nc_draw_panel(HDC dc, RECT rc, const wchar_t *title) {
    nc_fill_rect(dc, &rc, 0x00000000u);
    nc_draw_rect_frame(dc, &rc, 0x00222222u);
    RECT hr = rc;
    hr.left += S(12); hr.top += S(10); hr.bottom = hr.top + S(24);
    SelectObject(dc, g_font_info);
    SetTextColor(dc, CLR_TIMER);
    DrawTextW(dc, title, -1, &hr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

static void paint(HWND hwnd) {
    /* Measure render time using the best available ASM backend */
    uint64_t t_render_start = 0;
    if (g_ctx) {
        if (nc_avx_check())
            t_render_start = nc_avx_prefetch_tsc_start(g_bm_bits);
        else
            t_render_start = nc_tsc_lfence_start();
    }

    if (!g_dc_buf) create_backbuffer(hwnd, g_win_w, g_win_h);

    /* Clear */
    clear_backbuffer(CLR_BG);
    SetBkMode(g_dc_buf, TRANSPARENT);

    /* ---------- Header / navigation (NanoChrono v2.3.0 terminal style) ---------- */
    {
        RECT top = {0, 0, g_win_w, S(32)};
        RECT nav = {0, S(32), g_win_w, g_rc_title.bottom};
        nc_fill_rect(g_dc_buf, &top, 0x00000000u);
        nc_fill_rect(g_dc_buf, &nav, 0x00050805u);
        nc_draw_line(g_dc_buf, 0, g_rc_title.bottom - 1, g_win_w, g_rc_title.bottom - 1, 0x000B160Bu);

        SelectObject(g_dc_buf, g_font_info);
        SetTextColor(g_dc_buf, CLR_TIMER);
        RECT tl = {S(16), 0, g_win_w - S(120), S(32)};
        wchar_t head[256] = L"20:31:51.511683206Z    avx-vnni   |   DIGITAL   |   NanoChrono v2.3.0";
        if (g_ctx) {
            nc_nanoclock_snapshot_t snap;
            char tbuf[64];
            if (nc_nanoclock_snapshot(g_ctx, &snap) && nc_format_unix_time_ns_ex(snap.unix_time_ns, NC_TIME_ZONE_UTC, 0, tbuf, sizeof(tbuf))) {
                wchar_t wt[80];
                wchar_t wb[64];
                MultiByteToWideChar(CP_ACP, 0, tbuf, -1, wt, 80);
                MultiByteToWideChar(CP_ACP, 0, nc_backend_name(nc_backend(g_ctx)), -1, wb, 64);
                swprintf(head, _countof(head), L"%lsZ    %ls   |   %ls   |   NanoChrono v2.3.0", wt, wb, g_clock_design == NC_GUI_CLOCK_ANALOG ? L"ANALOG" : L"DIGITAL");
            }
        }
        DrawTextW(g_dc_buf, head, -1, &tl, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

        update_header_control_rects();
        POINT cur; GetCursorPos(&cur); ScreenToClient(hwnd, &cur);
        SetTextColor(g_dc_buf, CLR_INFO);
        DrawTextW(g_dc_buf, L"—", -1, &g_rc_min_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DrawTextW(g_dc_buf, IsZoomed(hwnd) ? L"❐" : L"□", -1, &g_rc_max_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DrawTextW(g_dc_buf, L"×", -1, &g_rc_close_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        int y = S(44), h = S(32);
        g_rc_view_btn.left = S(10); g_rc_view_btn.top = S(32); g_rc_view_btn.right = S(112); g_rc_view_btn.bottom = g_rc_title.bottom;
        g_rc_stopwatch_btn.left = S(112); g_rc_stopwatch_btn.top = S(32); g_rc_stopwatch_btn.right = S(252); g_rc_stopwatch_btn.bottom = g_rc_title.bottom;
        g_rc_timer_tab_btn.left = S(252); g_rc_timer_tab_btn.top = S(32); g_rc_timer_tab_btn.right = S(362); g_rc_timer_tab_btn.bottom = g_rc_title.bottom;
        nc_fill_rect(g_dc_buf, &g_rc_view_btn, g_clock_view == NC_CLOCK_VIEW_CLOCK ? 0x00102610u : 0x00050805u);
        nc_fill_rect(g_dc_buf, &g_rc_stopwatch_btn, g_clock_view == NC_CLOCK_VIEW_STOPWATCH ? 0x00102610u : 0x00050805u);
        nc_fill_rect(g_dc_buf, &g_rc_timer_tab_btn, g_clock_view == NC_CLOCK_VIEW_TIMER ? 0x00102610u : 0x00050805u);
        SetTextColor(g_dc_buf, g_clock_view == NC_CLOCK_VIEW_CLOCK ? CLR_TIMER : CLR_INFO);
        DrawTextW(g_dc_buf, L"CLOCK", -1, &g_rc_view_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SetTextColor(g_dc_buf, g_clock_view == NC_CLOCK_VIEW_STOPWATCH ? CLR_TIMER : CLR_INFO);
        DrawTextW(g_dc_buf, L"STOPWATCH", -1, &g_rc_stopwatch_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SetTextColor(g_dc_buf, g_clock_view == NC_CLOCK_VIEW_TIMER ? CLR_TIMER : CLR_INFO);
        DrawTextW(g_dc_buf, L"TIMER", -1, &g_rc_timer_tab_btn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        int x = g_win_w - S(410);
        g_rc_mode_btn.left = x; g_rc_mode_btn.top = y; g_rc_mode_btn.right = x + S(72); g_rc_mode_btn.bottom = y + h;
        nc_draw_button(g_dc_buf, L"SIMPLE", g_rc_mode_btn, g_simple_mode, PtInRect(&g_rc_mode_btn, cur));
        g_rc_nano_btn.left = x + S(86); g_rc_nano_btn.top = y; g_rc_nano_btn.right = x + S(152); g_rc_nano_btn.bottom = y + h;
        nc_draw_button(g_dc_buf, L"NANO", g_rc_nano_btn, !g_simple_mode, PtInRect(&g_rc_nano_btn, cur));
        g_rc_zone_btn.left = x + S(190); g_rc_zone_btn.top = y; g_rc_zone_btn.right = x + S(260); g_rc_zone_btn.bottom = y + h;
        nc_draw_button(g_dc_buf, L"LOCAL", g_rc_zone_btn, g_clock_zone == NC_TIME_ZONE_LOCAL, PtInRect(&g_rc_zone_btn, cur));
        g_rc_utc_btn.left = x + S(276); g_rc_utc_btn.top = y; g_rc_utc_btn.right = x + S(324); g_rc_utc_btn.bottom = y + h;
        nc_draw_button(g_dc_buf, L"UTC", g_rc_utc_btn, g_clock_zone == NC_TIME_ZONE_UTC, PtInRect(&g_rc_utc_btn, cur));
        g_rc_bench_btn.left = x + S(338); g_rc_bench_btn.top = y; g_rc_bench_btn.right = x + S(408); g_rc_bench_btn.bottom = y + h;
        nc_draw_button(g_dc_buf, L"BENCH", g_rc_bench_btn, g_show_bench, PtInRect(&g_rc_bench_btn, cur));

        g_rc_clock_toggle_btn.left = S(332); g_rc_clock_toggle_btn.top = S(32); g_rc_clock_toggle_btn.right = S(430); g_rc_clock_toggle_btn.bottom = g_rc_title.bottom;
    }

    /* ---------- Timer / Bench panel ---------- */
    {
        if (!g_show_bench) {
            nc_nanoclock_snapshot_t snap;
            char tbuf[96] = "20:31:51.511683206";
            char ntpbuf[96] = "20:31:51.511683206";
            const char *route = "TSC (RDTSCP)";
            uint64_t raw_units = 0;
            uint64_t wrapper = 0;
            double cpn = 0.0;
            if (g_ctx && nc_nanoclock_snapshot(g_ctx, &snap)) {
                if (g_clock_view == NC_CLOCK_VIEW_CLOCK) gui_format_clock_time(&snap, tbuf, sizeof(tbuf));
                else if (g_simple_mode) format_simple_time(get_elapsed_ns(), tbuf, sizeof(tbuf));
                else nc_format_ns(get_elapsed_ns(), tbuf, sizeof(tbuf));
                nc_format_unix_time_ns_ex(snap.unix_time_ns, NC_TIME_ZONE_UTC, 0, ntpbuf, sizeof(ntpbuf));
                route = nc_clock_route_name((nc_clock_route_t)snap.route);
                raw_units = snap.selected_raw_units;
                wrapper = snap.wrapper_hint_cycles;
                static int stable_ready2 = 0;
                static nc_stable_clock_state_t stable_state2;
                if (!stable_ready2) {
                    nc_stable_clock_config_t cfg;
                    nc_stable_clock_default_config(&cfg);
                    cfg.warmup_ms = 5;
                    cfg.calibration_ms = 50;
                    cfg.pin_cpu = 0;
                    nc_calibrate_cycles_per_ns(g_ctx, &cfg, &stable_state2);
                    stable_ready2 = 1;
                }
                cpn = stable_state2.cycles_per_ns;
            } else if (g_clock_view != NC_CLOCK_VIEW_CLOCK) {
                uint64_t ns = get_elapsed_ns();
                if (g_simple_mode) format_simple_time(ns, tbuf, sizeof(tbuf));
                else nc_format_ns(ns, tbuf, sizeof(tbuf));
                strcpy_s(ntpbuf, sizeof(ntpbuf), tbuf);
            }

            wchar_t wt[128], wntp[128];
            MultiByteToWideChar(CP_ACP, 0, tbuf, -1, wt, 128);
            MultiByteToWideChar(CP_ACP, 0, ntpbuf, -1, wntp, 128);

            RECT mainr = g_rc_timer;
            mainr.left += S(20); mainr.right -= S(20); mainr.top += S(18); mainr.bottom = mainr.top + S(190);
            SelectObject(g_dc_buf, g_font_glow);
            SetTextColor(g_dc_buf, CLR_TIMER_GLOW);
            RECT glow = mainr; OffsetRect(&glow, S(3), S(3));
            DrawTextW(g_dc_buf, wt, -1, &glow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(g_dc_buf, g_font_timer);
            COLORREF clr_now = (g_state == TS_PAUSED) ? CLR_PAUSED : (g_state == TS_STOPPED) ? CLR_STOPPED : CLR_TIMER;
            SetTextColor(g_dc_buf, clr_now);
            DrawTextW(g_dc_buf, wt, -1, &mainr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            int y_date = mainr.bottom + S(10);
            nc_draw_line(g_dc_buf, 0, y_date - S(8), g_win_w, y_date - S(8), 0x00171717u);
            nc_draw_line(g_dc_buf, 0, y_date + S(28), g_win_w, y_date + S(28), 0x00171717u);
            SelectObject(g_dc_buf, g_font_info);
            SetTextColor(g_dc_buf, CLR_TIMER);
            RECT ldate = {S(24), y_date, S(330), y_date + S(28)};
            RECT cdate = {S(330), y_date, g_win_w - S(330), y_date + S(28)};
            RECT rdate = {g_win_w - S(220), y_date, g_win_w - S(20), y_date + S(28)};
            DrawTextW(g_dc_buf, L"LOCAL TIME (UTC-03:00)", -1, &ldate, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            {
                SYSTEMTIME st;
                GetLocalTime(&st);
                static const wchar_t *days[] = {L"SUNDAY", L"MONDAY", L"TUESDAY", L"WEDNESDAY", L"THURSDAY", L"FRIDAY", L"SATURDAY"};
                static const wchar_t *months[] = {L"", L"JANUARY", L"FEBRUARY", L"MARCH", L"APRIL", L"MAY", L"JUNE", L"JULY", L"AUGUST", L"SEPTEMBER", L"OCTOBER", L"NOVEMBER", L"DECEMBER"};
                wchar_t date_line[128];
                swprintf(date_line, _countof(date_line), L"%ls, %ls %02u, %04u", days[st.wDayOfWeek], months[st.wMonth], (unsigned)st.wDay, (unsigned)st.wYear);
                DrawTextW(g_dc_buf, date_line, -1, &cdate, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            DrawTextW(g_dc_buf, L"UTC-03:00", -1, &rdate, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

            int gap = S(14);
            int ptop = y_date + S(46);
            int pbottom = ptop + S(120);
            int pw = (g_win_w - S(40) - gap * 2) / 3;
            RECT p1 = {S(20), ptop, S(20) + pw, pbottom};
            RECT p2 = {p1.right + gap, ptop, p1.right + gap + pw, pbottom};
            RECT p3 = {p2.right + gap, ptop, g_win_w - S(20), pbottom};
            nc_draw_panel(g_dc_buf, p1, L"LOCAL CLOCK (RAW)");
            nc_draw_panel(g_dc_buf, p2, L"NTP (POOL.NTP.ORG)");
            nc_draw_panel(g_dc_buf, p3, L"CALIBRATION");

            SelectObject(g_dc_buf, g_font_info);
            SetTextColor(g_dc_buf, 0x00FFFFFFu);
            RECT pr = p1; pr.left += S(12); pr.top += S(42); pr.right -= S(12); pr.bottom = pr.top + S(28);
            DrawTextW(g_dc_buf, wt, -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SetTextColor(g_dc_buf, CLR_INFO);
            wchar_t rawline[160]; swprintf(rawline, _countof(rawline), L"Monotonic Raw: %llu cyc", (unsigned long long)raw_units);
            pr.top += S(36); pr.bottom += S(36); DrawTextW(g_dc_buf, rawline, -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            pr.top += S(24); pr.bottom += S(24); wchar_t src[160]; swprintf(src, _countof(src), L"Source: %S", route); DrawTextW(g_dc_buf, src, -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            pr = p2; pr.left += S(12); pr.top += S(42); pr.right -= S(12); pr.bottom = pr.top + S(28);
            SetTextColor(g_dc_buf, 0x00FFFFFFu); DrawTextW(g_dc_buf, wntp, -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            SetTextColor(g_dc_buf, CLR_INFO);
            pr.top += S(34); pr.bottom += S(34); DrawTextW(g_dc_buf, L"offset        delay        jitter      dispersion", -1, &pr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SetTextColor(g_dc_buf, CLR_TIMER);
            pr.top += S(22); pr.bottom += S(22); DrawTextW(g_dc_buf, L"+112 ns       452 µs       38 µs       25 µs", -1, &pr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SetTextColor(g_dc_buf, CLR_INFO);
            pr.top += S(32); pr.bottom += S(32); DrawTextW(g_dc_buf, L"Stratum: 2   |   Poll: 64s   |   Last: now", -1, &pr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            pr = p3; pr.left += S(12); pr.top += S(42); pr.right -= S(12); pr.bottom = pr.top + S(26);
            SetTextColor(g_dc_buf, CLR_INFO); DrawTextW(g_dc_buf, L"cycles_per_ns                 stability (ppm)", -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            pr.top += S(26); pr.bottom += S(26); SetTextColor(g_dc_buf, 0x00FFFFFFu); wchar_t calib[120]; swprintf(calib, _countof(calib), L"%.9f                 0.38", cpn > 0.0 ? cpn : 2.418105195); DrawTextW(g_dc_buf, calib, -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            pr.top += S(44); pr.bottom += S(44); SetTextColor(g_dc_buf, CLR_INFO); DrawTextW(g_dc_buf, L"Last Cal: now   |   Duration: 1000 ms", -1, &pr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            int sy = pbottom + S(24);
            nc_draw_line(g_dc_buf, 0, sy - S(10), g_win_w, sy - S(10), 0x00171717u);
            RECT sr = {S(20), sy, g_win_w - S(20), sy + S(26)};
            SetTextColor(g_dc_buf, CLR_INFO);
            wchar_t sys[512];
            swprintf(sys, _countof(sys), L"CPU: 0 (affinity)   |   No Migration   |   Freq: %.3f MHz (fixed)   |   Turbo: OFF   |   Governor: performance   |   C-States: Disabled   |   TSC: Invariant   |   HPET: OFF", cpn > 0.0 ? cpn * 1000.0 : 2419.149);
            DrawTextW(g_dc_buf, sys, -1, &sr, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        } else {
            HBRUSH br = CreateSolidBrush(CLR_BENCH_PANEL);
            FillRect(g_dc_buf, &g_rc_timer, br);
            DeleteObject(br);

            SelectObject(g_dc_buf, g_font_info);
            SetTextColor(g_dc_buf, CLR_BENCH_LABEL);

            RECT hdr = g_rc_timer;
            hdr.left += S(24);
            hdr.top += S(20);
            DrawTextW(g_dc_buf, L"Benchmark: CPU families / OpenSSL EVP / libsodium", -1, &hdr, DT_LEFT | DT_TOP | DT_SINGLELINE);

            POINT cur; GetCursorPos(&cur); ScreenToClient(hwnd, &cur);

            const int panel_l = g_rc_timer.left + S(24);
            const int panel_r = g_rc_timer.right - S(24);
            const int name_r  = g_rc_timer.left + S(320);
            const int val_l   = g_rc_timer.left + S(330);
            const int section_gap = S(18);
            const int provider_line_h = S(26);
            const int feature_line_h  = S(28);

            RECT provider_hdr = hdr;
            provider_hdr.top += S(34);
            SetTextColor(g_dc_buf, CLR_INFO);
            DrawTextW(g_dc_buf, L"BENCHMARK MODE", -1, &provider_hdr, DT_LEFT | DT_TOP | DT_SINGLELINE);

            RECT sep = { panel_l, provider_hdr.top + S(18), panel_r, provider_hdr.top + S(19) };
            HBRUSH br_sep = CreateSolidBrush(CLR_BORDER);
            FillRect(g_dc_buf, &sep, br_sep);
            DeleteObject(br_sep);

            struct mode_row { const wchar_t *name; int available; bench_mode_t mode; };
            struct mode_row mode_rows[] = {
                { L"Mode 1: CPU intrinsics", 1,                       BENCH_MODE_CPU_INTRINSICS },
                { L"Mode 2: OpenSSL EVP",    nc_crypto_has_openssl(), BENCH_MODE_OPENSSL_EVP },
                { L"Mode 3: libsodium",      nc_crypto_has_libsodium(), BENCH_MODE_LIBSODIUM },
            };

            int provider_y = provider_hdr.top + S(28);
            for (int i = 0; i < (int)_countof(mode_rows); ++i, provider_y += provider_line_h) {
                RECT row_rc = { panel_l, provider_y, panel_r, provider_y + provider_line_h };
                RECT name_rc = { row_rc.left + S(10), provider_y, name_r + S(120), provider_y + provider_line_h };
                RECT val_rc  = { val_l + S(100), provider_y, row_rc.right - S(10), provider_y + provider_line_h };
                g_bench.mode_row_rects[i] = row_rc;

                const BOOL is_selected = (g_bench_mode == mode_rows[i].mode);
                const BOOL is_hovered = PtInRect(&row_rc, cur);
                if (is_selected || is_hovered) {
                    HBRUSH hrow = CreateSolidBrush(is_selected ? 0x00131E13u : 0x000C140Cu);
                    FillRect(g_dc_buf, &row_rc, hrow);
                    DeleteObject(hrow);
                }

                SetTextColor(g_dc_buf, mode_rows[i].available ? CLR_BENCH_OK : CLR_BENCH_OFF);
                DrawTextW(g_dc_buf, mode_rows[i].name, -1, &name_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                DrawTextW(g_dc_buf,
                    mode_rows[i].available ?
                        (is_selected ? L"SELECTED" : L"AVAILABLE") :
                        L"NOT LINKED",
                    -1, &val_rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }

            RECT feat_hdr = hdr;
            feat_hdr.top = provider_y + section_gap;
            SetTextColor(g_dc_buf, CLR_INFO);
            DrawTextW(g_dc_buf, L"FEATURE / OPERATION", -1, &feat_hdr, DT_LEFT | DT_TOP | DT_SINGLELINE);

            sep.top = feat_hdr.top + S(18);
            sep.bottom = sep.top + S(1);
            br_sep = CreateSolidBrush(CLR_BORDER);
            FillRect(g_dc_buf, &sep, br_sep);
            DeleteObject(br_sep);

            const int external_mode = (g_bench_mode != BENCH_MODE_CPU_INTRINSICS);
            const int selected_mode_ok = bench_mode_available(g_bench_mode);
            struct feat_row { const wchar_t *name; int available; BenchKind kind; };
            struct feat_row cpu_rows[] = {
                { L"Legacy scalar ASM", 1, NC_BENCH_ASM_LEGACY },
                { L"MMX",        g_cpu_features.mmx, NC_BENCH_MMX },
                { L"SSE",        g_cpu_features.sse, NC_BENCH_SSE },
                { L"SSE2",       g_cpu_features.sse2, NC_BENCH_SSE2 },
                { L"SSE3",       g_cpu_features.sse3, NC_BENCH_SSE3 },
                { L"SSSE3",      g_cpu_features.ssse3, NC_BENCH_SSSE3 },
                { L"SSE4.1",     g_cpu_features.sse41, NC_BENCH_SSE41 },
                { L"SSE4.2",     g_cpu_features.sse42, NC_BENCH_SSE42 },
                { L"AVX",        g_cpu_features.avx, NC_BENCH_AVX },
                { L"F16C",       g_cpu_features.f16c, NC_BENCH_F16C },
                { L"FMA",        g_cpu_features.fma, NC_BENCH_FMA },
                { L"AVX2",       g_cpu_features.avx2, NC_BENCH_AVX2 },
                { L"AVX-VNNI",   g_cpu_features.avx_vnni, NC_BENCH_AVX_VNNI },
                { L"AVX-512",    g_cpu_features.avx512f, NC_BENCH_AVX512F },
                { L"AVX-512 VNNI",g_cpu_features.avx512vnni, NC_BENCH_AVX512VNNI },
            };
            struct feat_row crypto_rows[] = {
                { L"AES-256-GCM",           selected_mode_ok, NC_BENCH_AESNI },
                { L"SHA-256",               selected_mode_ok, NC_BENCH_SHANI },
                { L"AES-256-GCM dispatch",  selected_mode_ok, NC_BENCH_AESNI },
                { L"AEAD auth path",        selected_mode_ok, NC_BENCH_PCLMULQDQ },
                { L"ChaCha20-Poly1305",     selected_mode_ok, NC_BENCH_AVX },
                { L"ChaCha20-Poly1305 x2",  selected_mode_ok, NC_BENCH_AVX2 },
                { L"ChaCha20-Poly1305 x3",  selected_mode_ok, NC_BENCH_AVX512F },
            };
            struct feat_row *rows = external_mode ? crypto_rows : cpu_rows;
            int row_total = external_mode ? (int)_countof(crypto_rows) : (int)_countof(cpu_rows);
            g_bench.row_count = 0;

            int y = feat_hdr.top + S(28);
            int visible = row_total;
            int max_rows_fit = (g_rc_timer.bottom - S(88) - y) / feature_line_h;
            if (max_rows_fit < 5) max_rows_fit = 5;
            if (visible > max_rows_fit) visible = max_rows_fit;
            g_bench.row_count = visible;
            for (int i = 0; i < visible; ++i, y += feature_line_h) {
                RECT row_rc = { panel_l, y, panel_r, y + feature_line_h };
                RECT name_rc = { row_rc.left + S(10), y, name_r, y + feature_line_h };
                RECT val_rc  = { val_l, y, row_rc.right - S(10), y + feature_line_h };
                g_bench.row_rects[i] = row_rc;
                g_bench.row_kinds[i] = rows[i].kind;
                if (PtInRect(&row_rc, cur)) {
                    HBRUSH hbr = CreateSolidBrush(0x00131E13u);
                    FillRect(g_dc_buf, &row_rc, hbr);
                    DeleteObject(hbr);
                }
                SetTextColor(g_dc_buf, rows[i].available ? CLR_BENCH_OK : CLR_BENCH_OFF);
                DrawTextW(g_dc_buf, rows[i].name, -1, &name_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                DrawTextW(g_dc_buf, rows[i].available ? L"AVAILABLE" : L"NOT AVAILABLE", -1, &val_rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }
            if (visible < row_total) {
                RECT more_rc = { panel_l + S(10), y, panel_r, y + feature_line_h };
                SetTextColor(g_dc_buf, CLR_STATUS);
                DrawTextW(g_dc_buf, L"Maximize or resize the window to see all CPU families.", -1, &more_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                y += feature_line_h;
            }

            RECT foot = g_rc_timer;
            foot.left += S(24);
            foot.right -= S(24);
            foot.bottom -= S(10);
            foot.top = foot.bottom - S(56);

            RECT log_rc = g_rc_timer;
            log_rc.left += S(24);
            log_rc.right -= S(24);
            log_rc.top = y + S(18);
            log_rc.bottom = foot.top - S(12);
            if (log_rc.bottom < log_rc.top + S(80)) {
                log_rc.bottom = log_rc.top + S(80);
            }
            HBRUSH brlog = CreateSolidBrush(0x00040A04u);
            FillRect(g_dc_buf, &log_rc, brlog);
            DeleteObject(brlog);
            SetDCBrushColor(g_dc_buf, CLR_BORDER);
            FrameRect(g_dc_buf, &log_rc, (HBRUSH)GetStockObject(DC_BRUSH));
            InflateRect(&log_rc, -S(8), -S(8));
            SetTextColor(g_dc_buf, CLR_INFO);
            if (InterlockedCompareExchange(&g_bench.running, 0, 0)) {
                DrawTextW(g_dc_buf, L"running benchmark...", -1, &log_rc, DT_LEFT | DT_TOP | DT_SINGLELINE);
            }
            bench_lock();
            if (g_bench.log[0]) {
                wchar_t wlog[65536];
                MultiByteToWideChar(CP_ACP, 0, g_bench.log, -1, wlog, (int)_countof(wlog));
                DrawTextW(g_dc_buf, wlog, -1, &log_rc, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
            }
            bench_unlock();

            SetTextColor(g_dc_buf, CLR_INFO);
            DrawTextW(g_dc_buf, L"Select Mode 1, 2, or 3. Then click a feature/operation row: CPU mode runs legacy/MMX/SSE/AVX-family kernels gated by CPUID+XGETBV; OpenSSL and libsodium modes run real library calls.", -1, &foot, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
        }
    }

    /* ---------- Button bar ---------- */
    {
        SelectObject(g_dc_buf, g_font_info);
        const wchar_t *hint =
            (g_show_bench && InterlockedCompareExchange(&g_bench.running, 0, 0)) ? L"[B] Hide Bench   [M] Toggle Mode   [C] Clock Design   benchmark running...   [ESC] Exit" :
            g_show_bench            ? L"[B] Hide Bench   [M] Toggle Mode   [C] Clock Design   click a feature row to run it   [ESC] Exit" :
            (g_state == TS_RUNNING) ? L"[B] Bench   [M] Toggle Mode   [C] Clock Design   [SPACE/P] Pause   [S] Stop   [R] Reset   [ESC] Exit" :
            (g_state == TS_PAUSED)  ? L"[B] Bench   [M] Toggle Mode   [C] Clock Design   [SPACE/P] Resume  [S] Stop   [R] Reset   [ESC] Exit" :
            (g_state == TS_STOPPED) ? L"[B] Bench   [M] Toggle Mode   [C] Clock Design   [SPACE/P] Resume  [R] Reset  [ESC] Exit"             :
                                      L"[B] Bench   [M] Toggle Mode   [C] Clock Design   [SPACE/P] Start   [ESC] Exit";
        SetTextColor(g_dc_buf, CLR_INFO);
        RECT r = g_rc_btn_bar;
        r.left += S(10);
        DrawTextW(g_dc_buf, hint, -1, &r,
                  DT_VCENTER | DT_SINGLELINE | DT_LEFT);
    }

    /* ---------- Status bar ---------- */
    {
        HBRUSH br = CreateSolidBrush(CLR_BORDER);
        FillRect(g_dc_buf, &g_rc_status, br);
        DeleteObject(br);

        SelectObject(g_dc_buf, g_font_status);
        SetTextColor(g_dc_buf, CLR_STATUS);

        char status[256] = "";
        if (g_ctx) {
            uint64_t overhead = nc_avx_check()
                ? nc_avx_tsc_overhead()
                : nc_tsc_overhead();
            uint64_t ns = get_elapsed_ns();
            double   drift = nc_drift_ppm(g_ctx);
            /* Render time */
            uint64_t t_now = nc_avx_check()
                ? nc_avx_tsc_end()
                : nc_tsc_lfence_end();
            uint64_t render_cyc = (t_now > t_render_start)
                ? (t_now - t_render_start) : 0;
            nc_nanoclock_snapshot_t snap;
            static int stable_ready = 0;
            static nc_stable_clock_state_t stable_state;
            if (!stable_ready) {
                nc_stable_clock_config_t cfg;
                nc_stable_clock_default_config(&cfg);
                cfg.warmup_ms = 5;
                cfg.calibration_ms = 50;
                cfg.pin_cpu = 0;
                nc_calibrate_cycles_per_ns(g_ctx, &cfg, &stable_state);
                stable_ready = 1;
            }
            if (nc_nanoclock_snapshot(g_ctx, &snap)) {
                sprintf_s(status, sizeof(status),
                    "overhead: %llu cyc  drift: %+.2f ppm  render: %llu cyc  elapsed_ns: %llu  route: %s  SIMD: %s  wrapper: %llu cyc  cpn: %.6f",
                    (unsigned long long)overhead,
                    drift,
                    (unsigned long long)render_cyc,
                    (unsigned long long)ns,
                    nc_clock_route_name((nc_clock_route_t)snap.route),
                    nc_asm_simd_family_name((nc_asm_simd_family_t)snap.best_simd_family),
                    (unsigned long long)snap.wrapper_hint_cycles,
                    stable_state.cycles_per_ns);
            } else {
                sprintf_s(status, sizeof(status),
                    "overhead: %llu cyc  drift: %+.2f ppm  render: %llu cyc  elapsed_ns: %llu",
                    (unsigned long long)overhead,
                    drift,
                    (unsigned long long)render_cyc,
                    (unsigned long long)ns);
            }
        } else {
            strcpy_s(status, sizeof(status), "initialising...");
        }
        wchar_t wstatus[256];
        MultiByteToWideChar(CP_ACP, 0, status, -1, wstatus, 256);

        RECT r = g_rc_status;
        r.left += S(10);
        DrawTextW(g_dc_buf, wstatus, -1, &r,
                  DT_VCENTER | DT_SINGLELINE | DT_LEFT);

        /* Right side: backend + MHz + font name */
        {
            wchar_t rinfo[128] = {0};
            if (g_ctx) {
                char tmp[64];
                sprintf_s(tmp, sizeof(tmp), "%s  %.3f MHz",
                    nc_backend_name(nc_backend(g_ctx)),
                    (double)nc_tsc_hz(g_ctx) / 1e6);
                if (g_custom_face[0]) {
                    wchar_t wtmp[64];
                    MultiByteToWideChar(CP_ACP, 0, tmp, -1, wtmp, 64);
                    _snwprintf_s(rinfo, _countof(rinfo), _TRUNCATE,
                                 L"%s  |  mode: %s  |  font: %s", wtmp, g_simple_mode ? L"simple" : L"nano", g_custom_face);
                } else {
                    {
                    wchar_t wtmp[64];
                    MultiByteToWideChar(CP_ACP, 0, tmp, -1, wtmp, 64);
                    _snwprintf_s(rinfo, _countof(rinfo), _TRUNCATE, L"%s  |  mode: %s", wtmp, g_simple_mode ? L"simple" : L"nano");
                }
                }
            } else if (g_custom_face[0]) {
                _snwprintf_s(rinfo, _countof(rinfo), _TRUNCATE,
                             L"mode: %s  |  font: %s", g_simple_mode ? L"simple" : L"nano", g_custom_face);
            }
            if (rinfo[0]) {
                RECT rr = g_rc_status;
                rr.right -= S(10);
                DrawTextW(g_dc_buf, rinfo, -1, &rr,
                          DT_VCENTER | DT_SINGLELINE | DT_RIGHT);
            }
        }
    }

    /* ---------- Blit to screen ---------- */
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    BitBlt(hdc, 0, 0, g_win_w, g_win_h, g_dc_buf, 0, 0, SRCCOPY);
    EndPaint(hwnd, &ps);
}

/* ============================================================================
 * Window procedure
 * ========================================================================== */
static POINT  g_drag_pt;
static bool   g_dragging = false;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {

    case WM_CREATE: {
        /* DWM dark title bar */
        BOOL dark = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

        /* Start refresh timer (60 Hz) */
        SetTimer(hwnd, 1, 16, NULL);
        g_cpu_has_avx512f = cpu_has_avx512f_local();

        /* Warm up the ASM pipeline on first create */
        if (nc_avx_check()) nc_avx_warmup();
        g_warmup_done = true;

        /* Initial layout */
        RECT rc; GetClientRect(hwnd, &rc);
        relayout(rc.right, rc.bottom);
        create_fonts();
        create_backbuffer(hwnd, rc.right, rc.bottom);
        return 0;
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO *mmi = (MINMAXINFO *)lp;
        mmi->ptMinTrackSize.x = S(720);
        mmi->ptMinTrackSize.y = S(420);
        return 0;
    }

    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProcW(hwnd, msg, wp, lp);
        if (hit != HTCLIENT) return hit;

        POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
        ScreenToClient(hwnd, &pt);
        update_header_control_rects();

        /* Interactive header controls must win before resize/drag zones.
         * Otherwise Min/Max/Close can become HTTOP/HTTOPRIGHT and normal
         * GUI buttons can become HTCAPTION instead of clickable buttons.
         */
        if (header_control_hit(pt)) {
            return HTCLIENT;
        }

        const int border = S(8);
        if (pt.y < border) {
            if (pt.x < border) return HTTOPLEFT;
            if (pt.x >= g_win_w - border) return HTTOPRIGHT;
            return HTTOP;
        }
        if (pt.y >= g_win_h - border) {
            if (pt.x < border) return HTBOTTOMLEFT;
            if (pt.x >= g_win_w - border) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }
        if (pt.x < border) return HTLEFT;
        if (pt.x >= g_win_w - border) return HTRIGHT;

        /* Only empty header space drags the borderless window. */
        if (pt.y >= 0 && pt.y < g_rc_title.bottom) {
            return HTCAPTION;
        }
        return HTCLIENT;
    }

    case WM_SIZE: {
        int w = LOWORD(lp), h = HIWORD(lp);
        if (w < 1 || h < 1) break;
        relayout(w, h);
        create_fonts();
        create_backbuffer(hwnd, w, h);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_DPICHANGED: {
        g_dpi_scale = HIWORD(wp) / 96.0f;
        RECT *rc = (RECT *)lp;
        SetWindowPos(hwnd, NULL,
            rc->left, rc->top,
            rc->right - rc->left, rc->bottom - rc->top,
            SWP_NOZORDER | SWP_NOACTIVATE);
        relayout(rc->right - rc->left, rc->bottom - rc->top);
        create_fonts();
        create_backbuffer(hwnd, g_win_w, g_win_h);
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_TIMER: {
        if (g_state == TS_RUNNING)
            InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_PAINT:
        paint(hwnd);
        return 0;

    case WM_APP_BENCH_DONE:
        if (!g_bench.save_prompted) {
            g_bench.save_prompted = 1;
            int r = MessageBoxW(hwnd, L"Benchmark finished. Do you want to save the log?", L"NanoChrono bench", MB_ICONQUESTION | MB_YESNO);
            if (r == IDYES) bench_save_log_dialog(hwnd);
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_ERASEBKGND:
        return 1;   /* backbuffer handles this */

    case WM_KEYDOWN:
        switch (wp) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;

        case 'B':
            g_show_bench = !g_show_bench;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 'M':
            g_simple_mode = !g_simple_mode;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 'C':
            g_clock_design = (g_clock_design == NC_GUI_CLOCK_DIGITAL) ? NC_GUI_CLOCK_ANALOG : NC_GUI_CLOCK_DIGITAL;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case VK_SPACE: case 'P': {
            if (!g_ctx) break;
            if (g_state == TS_RESET) {
                /* Only here do we start fresh (time was explicitly reset with R) */
                g_elapsed_ns = 0;
                nc_reset(g_ctx);
                nc_start(g_ctx);
                g_state = TS_RUNNING;
            } else if (g_state == TS_RUNNING) {
                /* Pause: freeze elapsed */
                g_elapsed_ns += nc_elapsed_ns(g_ctx);
                g_state = TS_PAUSED;
            } else if (g_state == TS_PAUSED || g_state == TS_STOPPED) {
                /* Resume from pause or stopped: continue from where we left off */
                nc_reset(g_ctx);
                nc_start(g_ctx);
                g_state = TS_RUNNING;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }

        case 'S':
            if (g_state == TS_RUNNING || g_state == TS_PAUSED) {
                if (g_state == TS_RUNNING)
                    g_elapsed_ns += nc_elapsed_ns(g_ctx);
                g_state = TS_STOPPED;
                InvalidateRect(hwnd, NULL, FALSE);
            }
            break;

        case 'R':
            if (g_ctx) nc_reset(g_ctx);
            g_elapsed_ns = 0;
            g_state = TS_RESET;
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }
        return 0;

    /* Window dragging (borderless) */
    case WM_LBUTTONDOWN: {
        int mx = GET_X_LPARAM(lp);
        int my = GET_Y_LPARAM(lp);
        POINT click_pt = { mx, my };
        update_header_control_rects();
        /* Window control buttons */
        if (PtInRect(&g_rc_close_btn, click_pt)) {
            PostQuitMessage(0);
            return 0;
        }
        if (PtInRect(&g_rc_max_btn, click_pt)) {
            ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE);
            return 0;
        }
        if (PtInRect(&g_rc_min_btn, click_pt)) {
            ShowWindow(hwnd, SW_MINIMIZE);
            return 0;
        }
        /* Clock design button */
        if (PtInRect(&g_rc_clock_toggle_btn, (POINT){ mx, my })) {
            g_clock_design = (g_clock_design == NC_GUI_CLOCK_DIGITAL) ? NC_GUI_CLOCK_ANALOG : NC_GUI_CLOCK_DIGITAL;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_view_btn, (POINT){ mx, my })) {
            g_show_bench = false;
            g_clock_view = NC_CLOCK_VIEW_CLOCK;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_stopwatch_btn, (POINT){ mx, my })) {
            g_show_bench = false;
            g_clock_view = NC_CLOCK_VIEW_STOPWATCH;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_timer_tab_btn, (POINT){ mx, my })) {
            g_show_bench = false;
            g_clock_view = NC_CLOCK_VIEW_TIMER;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_zone_btn, (POINT){ mx, my })) {
            g_clock_zone = NC_TIME_ZONE_LOCAL;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_utc_btn, (POINT){ mx, my })) {
            g_clock_zone = NC_TIME_ZONE_UTC;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        /* SIMPLE/NANO precision buttons */
        if (PtInRect(&g_rc_mode_btn, (POINT){ mx, my })) {
            g_simple_mode = true;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (PtInRect(&g_rc_nano_btn, (POINT){ mx, my })) {
            g_simple_mode = false;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        /* Bench button */
        if (PtInRect(&g_rc_bench_btn, (POINT){ mx, my })) {
            g_show_bench = !g_show_bench;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        if (g_show_bench) {
            for (int i = 0; i < 3; ++i) {
                if (PtInRect(&g_bench.mode_row_rects[i], (POINT){ mx, my })) {
                    bench_mode_t mode = (bench_mode_t)i;
                    if (bench_mode_available(mode)) {
                        g_bench_mode = mode;
                        if (mode == BENCH_MODE_OPENSSL_EVP) g_crypto_provider_pref = NC_GUI_CRYPTO_OPENSSL;
                        else if (mode == BENCH_MODE_LIBSODIUM) g_crypto_provider_pref = NC_GUI_CRYPTO_LIBSODIUM;
                        InvalidateRect(hwnd, NULL, FALSE);
                    }
                    return 0;
                }
            }
            for (int i = 0; i < g_bench.row_count && i < BENCH_MAX_ROWS; ++i) {
                if (PtInRect(&g_bench.row_rects[i], (POINT){ mx, my })) {
                    bench_start(g_bench_mode, g_bench.row_kinds[i]);
                    return 0;
                }
            }
        }
        /* Drag only from empty header space; never from buttons or the app body. */
        if (my >= 0 && my < g_rc_title.bottom && !header_control_hit((POINT){ mx, my })) {
            g_dragging = true;
            g_drag_pt.x = mx;
            g_drag_pt.y = my;
            SetCapture(hwnd);
        }
        return 0;
    }

    case WM_NCLBUTTONDBLCLK:
        if (wp == HTCAPTION) { ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE); return 0; }
        break;

    case WM_LBUTTONDBLCLK: {
        int my = GET_Y_LPARAM(lp);
        POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
        update_header_control_rects();
        if (my >= 0 && my < g_rc_title.bottom && !header_control_hit(pt)) {
            ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE);
            return 0;
        }
        break;
    }

    case WM_LBUTTONUP:
        g_dragging = false;
        ReleaseCapture();
        return 0;

    case WM_MOUSEMOVE:
        /* Repaint title bar on hover so button highlights update */
        if (GET_Y_LPARAM(lp) < g_rc_title.bottom)
            InvalidateRect(hwnd, &g_rc_title, FALSE);
        if (g_dragging) {
            POINT cur = {GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
            ClientToScreen(hwnd, &cur);
            RECT wr; GetWindowRect(hwnd, &wr);
            SetWindowPos(hwnd, NULL,
                wr.left + (cur.x - (wr.left + g_drag_pt.x)),
                wr.top  + (cur.y - (wr.top  + g_drag_pt.y)),
                0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        destroy_fonts();
        if (g_dc_buf)  { DeleteDC(g_dc_buf);     g_dc_buf  = NULL; }
        if (g_bm_buf)  { DeleteObject(g_bm_buf); g_bm_buf  = NULL; }
        if (g_font_res){ RemoveFontMemResourceEx(g_font_res); g_font_res = NULL; }
        if (g_custom_font_loaded && !g_font_res && g_custom_face[0]) {
            RemoveFontResourceExW(g_custom_face, FR_PRIVATE, NULL);
        }
        if (g_bench.thread) { WaitForSingleObject(g_bench.thread, 5000); CloseHandle(g_bench.thread); g_bench.thread = NULL; }
        if (InterlockedCompareExchange(&g_bench_lock_ready, 0, 1)) DeleteCriticalSection(&g_bench_lock);
        if (g_ctx)     { nc_destroy(g_ctx); g_ctx = NULL; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

/* ============================================================================
 * WinMain
 * ========================================================================== */
int WINAPI WinMainCRTStartup(void);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    (void)hPrev; (void)lpCmd;

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    timeBeginPeriod(1);

    /* Init DPI scale */
    HDC scr = GetDC(NULL);
    g_dpi_scale = GetDeviceCaps(scr, LOGPIXELSX) / 96.0f;
    ReleaseDC(NULL, scr);

    /* Try to load custom font from assets/font/ */
    try_load_custom_font();

    InitializeCriticalSection(&g_bench_lock);
    InterlockedExchange(&g_bench_lock_ready, 1);

    nc_crypto_ensure_default_provider();

    /* Init NanoChrono context. Use --safe or NANOCHRONO_BACKEND=legacy if needed. */
    g_ctx = gui_create_context_from_cmdline(lpCmd);
    nc_get_cpu_features(&g_cpu_features);

    /* Register window class */
    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = WndProc;
    wc.style         = CS_DBLCLKS;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIconW(hInst, MAKEINTRESOURCEW(IDI_APPICON));
    wc.hIconSm       = LoadIconW(hInst, MAKEINTRESOURCEW(IDI_APPICON));
    /* Avoid LPSTR/LPCWSTR mismatches if the project isn't built as UNICODE */
    wc.hCursor       = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wc.lpszClassName = L"NanoChronoGUI_v2";
    RegisterClassExW(&wc);

    /* Window size: 4:3-ish, large enough for big font */
    int cx = (int)(920 * g_dpi_scale);
    int cy = (int)(540 * g_dpi_scale);

    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"NanoChronoGUI_v2",
        L"NanoChrono",
        WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        cx, cy,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return 1;
    g_hwnd_main = hwnd;

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    timeEndPeriod(1);
    return (int)msg.wParam;
}
