@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

:: =============================================================================
::  build_all.bat  -  NanoChrono  Complete Unified Build System
::  ─────────────────────────────────────────────────────────────────────────
::  Outputs:
::    dist\nanochrono.dll            Legacy DLL   (CPUID + SSE2 backend)
::    dist\nanochrono.lib            Legacy import lib
::    dist\nanochrono_avx.dll        AVX DLL      (LFENCE + AVX backend)
::    dist\nanochrono_avx.lib        AVX import lib
::    dist\nanochrono_static.lib     Legacy static lib
::    dist\nanochrono_avx_static.lib AVX static lib
::    dist\nanochrono_gui.exe        GUI (both backends baked in, SIMD-clear,
::                                   prefetch-enhanced TSC, font auto-load)
::
::  New ASM exports (legacy):  nc_simd_zero_buf, nc_simd_copy_buf,
::    nc_simd_xor_checksum, nc_simd_sum_u64, nc_simd_nt_zero,
::    nc_simd_prefetch_loop, nc_tsc_prefetch_start, nc_tsc_prefetch_end,
::    nc_tsc_pause_start, nc_tsc_full_fence, nc_tsc_rdtscp_seq
::
::  New ASM exports (avx):  nc_avx_simd_zero_buf, nc_avx_simd_copy_buf,
::    nc_avx_simd_xor_checksum, nc_avx_simd_sum_u64, nc_avx_simd_nt_zero,
::    nc_avx_simd_prefetch_loop, nc_avx_prefetch_tsc_start/end,
::    nc_avx_tsc_pause_start, nc_avx_tsc_full_fence, nc_avx_tsc_rdtscp_seq,
::    nc_avx_check, nc_avx2_check
::
::  Prerequisites:
::    . NASM  in PATH           https://nasm.us
::    . Python 3  in PATH       (icon generation, stdlib only)
::    . x64 Native Tools Command Prompt for VS 2019/2022
::
::  Usage:
::    build_all.bat           Full build
::    build_all.bat LIBS      Build only DLLs + static libs
::    build_all.bat GUI       Build only GUI exe (assumes libs built)
::    build_all.bat CLEAN     Delete build artefacts
::    build_all.bat REBUILD   Clean + full build
:: =============================================================================

set "VERSION=1.1.0"
set "EXE_NAME=nanochrono_gui"

:: ── Parse arguments ──────────────────────────────────────────────────────────
set "BUILD_LIBS=1"
set "BUILD_GUI=1"

if /i "%1"=="CLEAN"   goto :do_clean
if /i "%1"=="REBUILD" ( call :do_clean & goto :start_build )
if /i "%1"=="LIBS"    set "BUILD_GUI=0"
if /i "%1"=="GUI"     set "BUILD_LIBS=0"

:start_build

echo.
echo  ╔═══════════════════════════════════════════════════════════════════╗
echo  ║     NANOCHRONO  v%VERSION%  -  Unified Build System                  ║
echo  ║                                                                   ║
echo  ║  Legacy ASM: CPUID, SSE2 MOVDQU, PREFETCH, PAUSE, RDTSCP        ║
echo  ║  AVX    ASM: LFENCE, VMOVDQU, VPXOR, VMOVNTDQ, VZEROUPPER       ║
echo  ╚═══════════════════════════════════════════════════════════════════╝
echo.

:: ── Tool verification ────────────────────────────────────────────────────────
echo [check] Verifying build tools...
where nasm  >nul 2>&1 || ( echo  [ERROR] NASM not found. https://nasm.us & exit /b 1 )
where cl    >nul 2>&1 || ( echo  [ERROR] cl.exe not found. Run from x64 Native Tools & exit /b 1 )
where lib   >nul 2>&1 || ( echo  [ERROR] lib.exe not found. & exit /b 1 )
where link  >nul 2>&1 || ( echo  [ERROR] link.exe not found. & exit /b 1 )
where rc    >nul 2>&1 || ( echo  [ERROR] rc.exe not found. Windows SDK required. & exit /b 1 )
echo  [check] Tools OK

:: ── Directory setup ──────────────────────────────────────────────────────────
if not exist obj          mkdir obj
if not exist dist         mkdir dist
if not exist assets       mkdir assets
if not exist assets\font  mkdir assets\font

:: ── Step 0: Icon ─────────────────────────────────────────────────────────────
echo.
echo  [0/9] Icon check...
if exist assets\nanochrono.ico (
    echo         assets\nanochrono.ico  already present
) else (
    where python >nul 2>&1
    if errorlevel 1 (
        echo  [WARN]  Python not found - creating stub icon
        copy /b nul assets\nanochrono.ico >nul
    ) else (
        python gen_ico.py
        if errorlevel 1 copy /b nul assets\nanochrono.ico >nul
    )
)

:: =============================================================================
::  ASSEMBLY PASS
:: =============================================================================
echo.
echo  [1/9] Assembling nanochrono_legacy.asm  (CPUID + SSE2 MOVDQU / PREFETCH)...
nasm -f win64 ^
     -DNASM_WIN64 ^
     nanochrono_legacy.asm ^
     -o obj\nc_legacy.obj
if errorlevel 1 ( echo  [FAILED] legacy ASM & exit /b 1 )
echo         obj\nc_legacy.obj  OK

echo  [2/9] Assembling nanochrono_avx.asm  (LFENCE / VMOVDQU / VPXOR / VMOVNTDQ)...
nasm -f win64 ^
     -DNASM_WIN64 ^
     nanochrono_avx.asm ^
     -o obj\nc_avx.obj
if errorlevel 1 ( echo  [FAILED] AVX ASM & exit /b 1 )
echo         obj\nc_avx.obj  OK

:: =============================================================================
::  C COMPILATION PASS
::  Flags:
::    /O2 /Oi /Ot   – speed optimisations (inline, favour speed)
::    /GL           – whole-program optimisation (link-time)
::    /GS-          – no security cookies (bare-metal lib)
::    /arch:AVX2    – allow compiler to emit AVX2 intrinsics in .c code
::    /fp:fast      – fast FP (used in drift_ppm calc)
::    /Zc:inline    – remove unreferenced COMDAT
:: =============================================================================
set "CF=/nologo /W3 /WX- /O2 /Oi /Ot /GL /GS- /arch:AVX2 /fp:fast /std:c11 /utf-8 /Zc:inline"

echo.
echo  [3/9] Compiling nanochrono.c -> legacy DLL object...
cl %CF% /DNC_LEGACY_BUILD /DNANOCHRONO_DLL /DNANOCHRONO_EXPORTS ^
   /c nanochrono.c /Fo:obj\nc_c_leg.obj
if errorlevel 1 ( echo  [FAILED] nc_c_leg.obj & exit /b 1 )
echo         obj\nc_c_leg.obj  OK

echo  [4/9] Compiling nanochrono.c -> AVX DLL object...
cl %CF% /DNC_AVX_BUILD /DNANOCHRONO_DLL /DNANOCHRONO_EXPORTS ^
   /c nanochrono.c /Fo:obj\nc_c_avx.obj
if errorlevel 1 ( echo  [FAILED] nc_c_avx.obj & exit /b 1 )
echo         obj\nc_c_avx.obj  OK

echo  [5/9] Compiling nanochrono.c -> unified exe object (runtime dispatch)...
cl %CF% /DNC_UNIFIED_BUILD ^
   /c nanochrono.c /Fo:obj\nc_c_uni.obj
if errorlevel 1 ( echo  [FAILED] nc_c_uni.obj & exit /b 1 )
echo         obj\nc_c_uni.obj  OK

:: =============================================================================
::  LIBRARY LINK PASS
:: =============================================================================
if "%BUILD_LIBS%"=="0" goto :skip_libs

echo.
echo  [6/9] Linking libraries...

echo         nanochrono.dll  (legacy CPUID + SSE2)
link /nologo /DLL /OUT:dist\nanochrono.dll /IMPLIB:dist\nanochrono.lib ^
     /DEF:nanochrono_legacy.def /LTCG /OPT:REF /OPT:ICF /SUBSYSTEM:WINDOWS ^
     obj\nc_legacy.obj obj\nc_c_leg.obj kernel32.lib
if errorlevel 1 ( echo  [FAILED] nanochrono.dll & exit /b 1 )

echo         nanochrono_avx.dll  (LFENCE + AVX VMOVDQU)
link /nologo /DLL /OUT:dist\nanochrono_avx.dll /IMPLIB:dist\nanochrono_avx.lib ^
     /DEF:nanochrono_avx.def /LTCG /OPT:REF /OPT:ICF /SUBSYSTEM:WINDOWS ^
     obj\nc_avx.obj obj\nc_c_avx.obj kernel32.lib
if errorlevel 1 ( echo  [FAILED] nanochrono_avx.dll & exit /b 1 )

echo         nanochrono_static.lib
lib /nologo /OUT:dist\nanochrono_static.lib obj\nc_legacy.obj obj\nc_c_leg.obj
if errorlevel 1 ( echo  [FAILED] nanochrono_static.lib & exit /b 1 )

echo         nanochrono_avx_static.lib
lib /nologo /OUT:dist\nanochrono_avx_static.lib obj\nc_avx.obj obj\nc_c_avx.obj
if errorlevel 1 ( echo  [FAILED] nanochrono_avx_static.lib & exit /b 1 )

echo  [6/9] Libraries done.

:skip_libs

:: =============================================================================
::  GUI EXE BUILD PASS
::  Links BOTH ASM backends + unified C layer + main.c
::  main.c uses nc_avx_simd_zero_buf / nc_simd_zero_buf for backbuffer clear,
::  nc_avx_prefetch_tsc_start / nc_avx_tsc_end for render tracing.
:: =============================================================================
if "%BUILD_GUI%"=="0" goto :skip_gui

echo.
:: ── Auto-detect font and embed as resource ───────────────────────────────────
echo  [font] Scanning assets\font\ for TTF/OTF to embed...
set "FONT_RC_OBJ="
set "FONT_RC_ARG="

for %%F in (assets\font\*.ttf assets\font\*.otf) do (
    if not defined FONT_FILE (
        set "FONT_FILE=%%F"
        echo         Found: %%F
    )
)

if defined FONT_FILE (
    echo  [font] Generating font_embed.rc ^& compiling...
    echo #include "resource.h"                          > obj\font_embed.rc
    echo IDR_CUSTOM_FONT RCDATA "%FONT_FILE%"          >> obj\font_embed.rc
    rc /nologo /I. /fo obj\font_embed.res obj\font_embed.rc
    if errorlevel 1 (
        echo  [WARN] Font RC compile failed - building without embedded font
        set "FONT_RC_OBJ="
    ) else (
        set "FONT_RC_OBJ=obj\font_embed.res"
        echo         Embedded successfully.
    )
) else (
    echo  [font] No font found - using Consolas fallback.
)

echo  [7/9] Compiling main.c  (GUI, static, SIMD-optimised backbuffer)...
cl %CF% /DNC_UNIFIED_BUILD /I. ^
   /c main.c /Fo:obj\nc_main.obj
if errorlevel 1 ( echo  [FAILED] nc_main.obj & exit /b 1 )
echo         obj\nc_main.obj  OK

echo  [8/9] Compiling app.rc...
rc /nologo /I. /fo obj\nc_res.res app.rc
if errorlevel 1 ( echo  [FAILED] nc_res.res & exit /b 1 )
echo         obj\nc_res.res  OK

echo  [9/9] Linking dist\%EXE_NAME%.exe ...
link /nologo /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup ^
     /OUT:dist\%EXE_NAME%.exe ^
     /LTCG /OPT:REF /OPT:ICF ^
     obj\nc_legacy.obj ^
     obj\nc_avx.obj ^
     obj\nc_c_uni.obj ^
     obj\nc_main.obj ^
     obj\nc_res.res ^
     %FONT_RC_OBJ% ^
     kernel32.lib user32.lib gdi32.lib msimg32.lib dwmapi.lib winmm.lib
if errorlevel 1 ( echo  [FAILED] %EXE_NAME%.exe & exit /b 1 )

:: ── Copy assets next to exe so the font loader finds them ────────────────────
if "%BUILD_GUI%"=="0" goto :skip_gui_assets
echo  [+] Copying assets\ -^> distssets\  (font loader needs this next to .exe)
if not exist distssets      mkdir distssets
if not exist distssetsont mkdir distssetsont
xcopy /E /I /Y assets distssets ^>nul
echo         distssets\  OK
:skip_gui_assets

:skip_gui

:: ─────────────────────────────────────────────────────────────────────────────
::  REPORT
:: ─────────────────────────────────────────────────────────────────────────────
echo.
echo  ╔═══════════════════════════════════════════════════════════════════╗
echo  ║  Build complete                                                   ║
echo  ╠═══════════════════════════════════════════════════════════════════╣
echo  ║  DLLs:                                                            ║
call :size_line "  ║   " "dist\nanochrono.dll"             "legacy CPUID+SSE2  "
call :size_line "  ║   " "dist\nanochrono_avx.dll"         "AVX LFENCE+VMOVDQU "
echo  ║  Static libs:                                                     ║
call :size_line "  ║   " "dist\nanochrono_static.lib"       "legacy             "
call :size_line "  ║   " "dist\nanochrono_avx_static.lib"   "AVX                "
echo  ║  GUI exe (both backends + SIMD backbuffer + custom font):        ║
call :size_line "  ║   " "dist\nanochrono_gui.exe"           "runtime dispatch   "
echo  ╠═══════════════════════════════════════════════════════════════════╣
echo  ║  Controls:                                                        ║
echo  ║    SPACE / P  ->  Start / Pause / Resume                         ║
echo  ║    S          ->  Stop  (keeps elapsed)                           ║
echo  ║    R          ->  Reset to 00:00:00:000:000:000                  ║
echo  ║    ESC        ->  Exit                                            ║
echo  ║    Drag       ->  Move window                                     ║
echo  ║  Custom font: drop a .ttf or .otf into assets\font\              ║
echo  ╚═══════════════════════════════════════════════════════════════════╝
echo.
goto :eof

:size_line
set "SL_FILE=%~2"
if not exist "%SL_FILE%" ( echo %~1  %-48s  ^(not built^) & goto :eof )
for %%F in ("%SL_FILE%") do set /a SL_KB=%%~zF / 1024
echo %~1  %-48s  %~3  !SL_KB! KB
goto :eof

:do_clean
echo Cleaning...
if exist obj  rd /s /q obj
if exist dist rd /s /q dist
echo Done.
goto :eof
