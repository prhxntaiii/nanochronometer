cmake_minimum_required(VERSION 3.20)
project(NanoChrono C ASM_NASM RC)

# ── 64-bit only ───────────────────────────────────────────────────────────────
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "NanoChrono requires x64.")
endif()

# ── NASM ──────────────────────────────────────────────────────────────────────
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)
set(CMAKE_ASM_NASM_COMPILE_OBJECT
    "<CMAKE_ASM_NASM_COMPILER> -f win64 <SOURCE> -o <OBJECT>")

# ── MSVC flags ────────────────────────────────────────────────────────────────
if (MSVC)
    add_compile_options(/nologo /W3 /O2 /Oi /Ot /GL /GS- /arch:AVX2 /fp:fast /std:c11)
    add_link_options(/nologo /LTCG /OPT:REF /OPT:ICF)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# ASM object libraries  (private, no exported symbols on CMake side)
# ─────────────────────────────────────────────────────────────────────────────
add_library(nc_asm_legacy OBJECT nanochrono_legacy.asm)
add_library(nc_asm_avx    OBJECT nanochrono_avx.asm)

# ─────────────────────────────────────────────────────────────────────────────
# nanochrono.dll  –  Legacy CPUID backend
# ─────────────────────────────────────────────────────────────────────────────
add_library(nanochrono SHARED nanochrono.c)
target_sources(nanochrono PRIVATE $<TARGET_OBJECTS:nc_asm_legacy>)
target_compile_definitions(nanochrono PRIVATE
    NC_LEGACY_BUILD NANOCHRONO_DLL NANOCHRONO_EXPORTS)
set_target_properties(nanochrono PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF
    LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/nanochrono_legacy.def")
target_include_directories(nanochrono PUBLIC .)

# ─────────────────────────────────────────────────────────────────────────────
# nanochrono_avx.dll  –  AVX LFENCE backend
# ─────────────────────────────────────────────────────────────────────────────
add_library(nanochrono_avx SHARED nanochrono.c)
target_sources(nanochrono_avx PRIVATE $<TARGET_OBJECTS:nc_asm_avx>)
target_compile_definitions(nanochrono_avx PRIVATE
    NC_AVX_BUILD NANOCHRONO_DLL NANOCHRONO_EXPORTS)
set_target_properties(nanochrono_avx PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF
    LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/nanochrono_avx.def")
target_include_directories(nanochrono_avx PUBLIC .)

# ─────────────────────────────────────────────────────────────────────────────
# nanochrono_static.lib  –  Legacy static
# ─────────────────────────────────────────────────────────────────────────────
add_library(nanochrono_static STATIC nanochrono.c)
target_sources(nanochrono_static PRIVATE $<TARGET_OBJECTS:nc_asm_legacy>)
target_compile_definitions(nanochrono_static PRIVATE NC_LEGACY_BUILD)
target_include_directories(nanochrono_static PUBLIC .)

# ─────────────────────────────────────────────────────────────────────────────
# nanochrono_avx_static.lib  –  AVX static
# ─────────────────────────────────────────────────────────────────────────────
add_library(nanochrono_avx_static STATIC nanochrono.c)
target_sources(nanochrono_avx_static PRIVATE $<TARGET_OBJECTS:nc_asm_avx>)
target_compile_definitions(nanochrono_avx_static PRIVATE NC_AVX_BUILD)
target_include_directories(nanochrono_avx_static PUBLIC .)

# ─────────────────────────────────────────────────────────────────────────────
# Icon generation  (Python stdlib only – no Pillow)
# ─────────────────────────────────────────────────────────────────────────────
find_program(PYTHON_EXE NAMES python3 python)
set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/nanochrono.ico")
if (PYTHON_EXE)
    add_custom_command(
        OUTPUT  "${ICON_FILE}"
        COMMAND ${PYTHON_EXE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_ico.py"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "[NanoChrono] Generating assets/nanochrono.ico"
        VERBATIM)
    add_custom_target(nanochrono_icon DEPENDS "${ICON_FILE}")
else()
    message(WARNING "Python not found – drop assets/nanochrono.ico manually.")
endif()

# ─────────────────────────────────────────────────────────────────────────────
# nanochrono_gui.exe
#   · Links BOTH ASM backends statically
#   · nanochrono.c compiled as NC_UNIFIED_BUILD (runtime dispatch)
#   · src/main.c compiled without NANOCHRONO_DLL (pure static call)
# ─────────────────────────────────────────────────────────────────────────────

# Unified C object (runtime dispatch, no DLL decoration)
add_library(nc_c_unified OBJECT nanochrono.c)
target_compile_definitions(nc_c_unified PRIVATE NC_UNIFIED_BUILD)
target_include_directories(nc_c_unified PUBLIC .)

add_executable(nanochrono_gui WIN32 src/main.c src/app.rc)
target_sources(nanochrono_gui PRIVATE
    $<TARGET_OBJECTS:nc_asm_legacy>
    $<TARGET_OBJECTS:nc_asm_avx>
    $<TARGET_OBJECTS:nc_c_unified>)
target_include_directories(nanochrono_gui PRIVATE . src)
target_compile_definitions(nanochrono_gui PRIVATE NC_UNIFIED_BUILD)
target_link_libraries(nanochrono_gui PRIVATE
    dwmapi winmm msimg32)
if (MSVC)
    set_target_properties(nanochrono_gui PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup /MANIFEST:EMBED")
endif()
if (PYTHON_EXE)
    add_dependencies(nanochrono_gui nanochrono_icon)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Console demo
# ─────────────────────────────────────────────────────────────────────────────
if (BUILD_DEMO)
    add_executable(nc_demo demo.c)
    target_sources(nc_demo PRIVATE
        $<TARGET_OBJECTS:nc_asm_legacy>
        $<TARGET_OBJECTS:nc_asm_avx>
        $<TARGET_OBJECTS:nc_c_unified>)
    target_include_directories(nc_demo PRIVATE .)
    target_compile_definitions(nc_demo PRIVATE NC_UNIFIED_BUILD)
    target_link_libraries(nc_demo PRIVATE kernel32)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Install
# ─────────────────────────────────────────────────────────────────────────────
install(TARGETS
    nanochrono nanochrono_avx
    nanochrono_static nanochrono_avx_static
    nanochrono_gui
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(FILES nanochrono.h DESTINATION include)
