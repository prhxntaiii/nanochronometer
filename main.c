/* =============================================================================
 * main.c  -  NanoChrono GUI  (Win32, no CRT, static backend)
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
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <mmsystem.h>

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

/* ============================================================================
 * Colours
 * ========================================================================== */
#ifndef CLR_BG
#define CLR_BG          0x00000000u
#endif
#define CLR_TIMER       0x0033FF88u   /* bright green-cyan */
#define CLR_TIMER_GLOW  0x00114422u   /* dark glow underlay */
#define CLR_PAUSED      0x0000AAFFu   /* blue when paused  */
#define CLR_STOPPED     0x00AA4400u   /* amber when stopped */
#define CLR_INFO        0x0099CC99u
#define CLR_STATUS      0x0077AA77u
#define CLR_BTN_ACTIVE  0x0044BB66u
#ifndef CLR_BORDER
#define CLR_BORDER      0x00162416u
#endif

/* ============================================================================
 * Timer state
 * ========================================================================== */
typedef enum { TS_RESET = 0, TS_RUNNING, TS_PAUSED, TS_STOPPED } TimerState;

static nc_ctx_t   *g_ctx        = NULL;
static TimerState  g_state      = TS_RESET;
static uint64_t    g_elapsed_ns = 0;   /* frozen ns when paused/stopped */

/* ============================================================================
 * Layout
 * ========================================================================== */
static int  g_win_w = 920, g_win_h = 540;
static RECT g_rc_title, g_rc_timer, g_rc_btn_bar, g_rc_status;
static float g_dpi_scale = 1.0f;
static int S(int v) { return (int)(v * g_dpi_scale + 0.5f); }

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
    g_rc_title.top  = 0; g_rc_title.bottom = S(34);

    /* Status bar */
    g_rc_status.left   = 0; g_rc_status.right  = w;
    g_rc_status.bottom = h; g_rc_status.top    = h - S(22);

    /* Button bar just above status */
    g_rc_btn_bar.left   = 0; g_rc_btn_bar.right  = w;
    g_rc_btn_bar.bottom = g_rc_status.top;
    g_rc_btn_bar.top    = g_rc_btn_bar.bottom - S(28);

    /* Timer occupies the middle */
    g_rc_timer.left   = S(12);
    g_rc_timer.right  = w - S(12);
    g_rc_timer.top    = g_rc_title.bottom + S(8);
    g_rc_timer.bottom = g_rc_btn_bar.top  - S(8);
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

    /* ---------- Title bar ---------- */
    {
        HBRUSH br = CreateSolidBrush(CLR_BORDER);
        FillRect(g_dc_buf, &g_rc_title, br);
        DeleteObject(br);

        SelectObject(g_dc_buf, g_font_info);
        SetTextColor(g_dc_buf, CLR_INFO);

        /* App name left */
        RECT r = g_rc_title;
        r.left  += S(10);
        r.right -= S(70);   /* leave room for buttons */
        DrawTextW(g_dc_buf, L"NanoChrono", -1, &r,
                  DT_VCENTER | DT_SINGLELINE | DT_LEFT);

        /* Minimize / Close buttons right-aligned */
        int btn_h  = g_rc_title.bottom - g_rc_title.top;
        int btn_w  = S(34);

        /* Close button [X] */
        RECT rc_close = { g_rc_title.right - btn_w, 0,
                          g_rc_title.right,          btn_h };
        /* Hover detection */
        POINT cur; GetCursorPos(&cur);
        ScreenToClient(hwnd, &cur);
        BOOL hov_close = PtInRect(&rc_close, cur);
        BOOL hov_min   = FALSE;

        RECT rc_min = { g_rc_title.right - btn_w * 2, 0,
                        g_rc_title.right - btn_w,      btn_h };
        hov_min = PtInRect(&rc_min, cur);

        /* Draw close */
        if (hov_close) {
            HBRUSH brc = CreateSolidBrush(0x002222CCu);
            FillRect(g_dc_buf, &rc_close, brc);
            DeleteObject(brc);
            SetTextColor(g_dc_buf, 0x00FFFFFFu);
        } else {
            SetTextColor(g_dc_buf, CLR_INFO);
        }
        DrawTextW(g_dc_buf, L"✕", -1, &rc_close,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        /* Draw minimize */
        if (hov_min) {
            HBRUSH brm = CreateSolidBrush(0x00224422u);
            FillRect(g_dc_buf, &rc_min, brm);
            DeleteObject(brm);
            SetTextColor(g_dc_buf, 0x00FFFFFFu);
        } else {
            SetTextColor(g_dc_buf, CLR_INFO);
        }
        DrawTextW(g_dc_buf, L"—", -1, &rc_min,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    /* ---------- Timer ---------- */
    {
        uint64_t ns = get_elapsed_ns();
        char     buf[32];
        nc_format_ns(ns, buf, sizeof(buf));
        wchar_t  wbuf[32];
        MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 32);

        COLORREF clr_now =
            (g_state == TS_PAUSED)  ? CLR_PAUSED  :
            (g_state == TS_STOPPED) ? CLR_STOPPED :
            CLR_TIMER;

        /* Glow underlay (+2,+2 offset, dark colour) */
        SelectObject(g_dc_buf, g_font_glow);
        SetTextColor(g_dc_buf, CLR_TIMER_GLOW);
        RECT gr = g_rc_timer;
        OffsetRect(&gr, S(2), S(2));
        DrawTextW(g_dc_buf, wbuf, -1, &gr,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        /* Main timer text */
        SelectObject(g_dc_buf, g_font_timer);
        SetTextColor(g_dc_buf, clr_now);
        RECT tr = g_rc_timer;
        DrawTextW(g_dc_buf, wbuf, -1, &tr,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    /* ---------- Button bar ---------- */
    {
        SelectObject(g_dc_buf, g_font_info);
        const wchar_t *hint =
            (g_state == TS_RUNNING) ? L"[SPACE/P] Pause   [S] Stop   [R] Reset   [ESC] Exit" :
            (g_state == TS_PAUSED)  ? L"[SPACE/P] Resume  [S] Stop   [R] Reset   [ESC] Exit" :
            (g_state == TS_STOPPED) ? L"[SPACE/P] Resume  [R] Reset  [ESC] Exit"              :
                                      L"[SPACE/P] Start   [ESC] Exit";
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

        char status[128] = "";
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
            sprintf_s(status, sizeof(status),
                "overhead: %llu cyc  drift: %+.2f ppm  render: %llu cyc  ns: %llu",
                (unsigned long long)overhead,
                drift,
                (unsigned long long)render_cyc,
                (unsigned long long)ns);
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
                    nc_backend(g_ctx) == NC_BACKEND_AVX ? "AVX" : "CPUID",
                    (double)nc_tsc_hz(g_ctx) / 1e6);
                if (g_custom_face[0]) {
                    wchar_t wtmp[64];
                    MultiByteToWideChar(CP_ACP, 0, tmp, -1, wtmp, 64);
                    _snwprintf_s(rinfo, _countof(rinfo), _TRUNCATE,
                                 L"%s  |  font: %s", wtmp, g_custom_face);
                } else {
                    MultiByteToWideChar(CP_ACP, 0, tmp, -1, rinfo, _countof(rinfo));
                }
            } else if (g_custom_face[0]) {
                _snwprintf_s(rinfo, _countof(rinfo), _TRUNCATE,
                             L"font: %s", g_custom_face);
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

    case WM_ERASEBKGND:
        return 1;   /* backbuffer handles this */

    case WM_KEYDOWN:
        switch (wp) {
        case VK_ESCAPE:
            PostQuitMessage(0);
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
                    g_elapsed_ns = nc_elapsed_ns(g_ctx);
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
        int btn_h = g_rc_title.bottom - g_rc_title.top;
        int btn_w = S(34);
        /* Close button */
        if (mx >= g_win_w - btn_w && my < btn_h) {
            PostQuitMessage(0);
            return 0;
        }
        /* Minimize button */
        if (mx >= g_win_w - btn_w * 2 && mx < g_win_w - btn_w && my < btn_h) {
            ShowWindow(hwnd, SW_MINIMIZE);
            return 0;
        }
        g_dragging = true;
        g_drag_pt.x = mx;
        g_drag_pt.y = my;
        SetCapture(hwnd);
        return 0;
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

    /* Init NanoChrono context (auto picks best backend) */
    g_ctx = nc_create();

    /* Register window class */
    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = WndProc;
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
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        cx, cy,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return 1;

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
