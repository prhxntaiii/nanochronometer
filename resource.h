/* =============================================================================
 * resource.h  –  NanoChrono GUI resource IDs
 * ============================================================================= */
#pragma once

/* Icon */
#define IDI_NANOCHRONO    101

/* Menu / Accelerators */
#define IDR_ACCEL         200
#define IDR_CUSTOM_FONT   201  /* RCDATA: embedded TTF/OTF, absent if no font found */

/* Controls */
#define IDC_BTN_PAUSE     301
#define IDC_BTN_STOP      302
#define IDC_BTN_RESET     303
#define IDC_BTN_EXIT      304

/* Timer */
#define IDT_UPDATE        1001

/* Layout constants (pixels, 96 DPI base) */
#define TITLEBAR_H        44
#define TIMER_AREA_H      160
#define INFO_AREA_H       36
#define SEP_H             2
#define BTN_AREA_H        72
#define STATUS_H          28

#define WIN_W             880
#define WIN_H             (TITLEBAR_H + TIMER_AREA_H + INFO_AREA_H + SEP_H + BTN_AREA_H + STATUS_H)

/* Colors  (0x00BBGGRR – GDI COLORREF format) */
#define CLR_BG            0x00080C08
#define CLR_PANEL         0x000E150E
#define CLR_CARD          0x00121912
#define CLR_ACCENT        0x0041FF00   /* phosphor green  */
#define CLR_ACCENT_DIM    0x00008A00   /* mid green       */
#define CLR_ACCENT_DARK   0x00003000   /* deep green glow */
#define CLR_ACCENT_STOP   0x000000CC   /* red (stopped)   */
#define CLR_ACCENT_PAUSE  0x0000AAFF   /* amber (paused)  */
#define CLR_TEXT          0x00C8F0C8
#define CLR_DIM           0x00406040
#define CLR_BORDER        0x00204020
#define CLR_BTN_BG        0x00101810
#define CLR_BTN_HOV       0x001A2A1A
#define CLR_BTN_PRE       0x00080E08
#define CLR_TITLE_TEXT    0x0090C090
#define CLR_STATUS_BG     0x00060A06
