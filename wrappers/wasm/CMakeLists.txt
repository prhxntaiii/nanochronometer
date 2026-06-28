cmake_minimum_required(VERSION 3.20)
project(NanoChronometer_WASM C)

# ─────────────────────────────────────────────────────────────────────────────
# Build system for WASM/WASI targets.
# Usage:
#   Emscripten:
#     emcmake cmake -B build_wasm wrappers/wasm -DCMAKE_BUILD_TYPE=Release
#     cmake --build build_wasm
#
#   WASI-SDK (standalone WASM):
#     cmake -B build_wasi wrappers/wasm \
#       -DCMAKE_TOOLCHAIN_FILE=/opt/wasi-sdk/share/cmake/wasi-sdk.cmake \
#       -DWASI_SDK_PREFIX=/opt/wasi-sdk
#     cmake --build build_wasi
# ─────────────────────────────────────────────────────────────────────────────

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-O2 -Wall -Wextra)

add_library(nanochrono_wasm STATIC nanochrono_wasm.c)
target_compile_definitions(nanochrono_wasm PRIVATE __wasm__=1)

if(EMSCRIPTEN)
    # Browser-targeted JS+WASM bundle
    set(EXPORTED_FUNCTIONS
        "_nc_wasm_now_ns"
        "_nc_wasm_realtime_ns"
        "_nc_wasm_start"
        "_nc_wasm_elapsed_ns"
        "_nc_wasm_feature_available"
        "_nc_wasm_version"
        "_nc_wasm_measure_aesenc_cycles"
        "_nc_wasm_measure_sha256msg_cycles"
        "_nc_wasm_crypto_sha256_cycles"
        "_nc_wasm_crypto_rand_cycles"
    )
    string(JOIN "," EXPORTED_LIST ${EXPORTED_FUNCTIONS})
    add_executable(nanochrono_browser nanochrono_wasm.c)
    target_compile_definitions(nanochrono_browser PRIVATE __wasm__=1)
    set_target_properties(nanochrono_browser PROPERTIES
        SUFFIX ".js"
        LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[${EXPORTED_LIST}]' -s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]' -s MODULARIZE=1 -s EXPORT_NAME=NanoChronometer")
    # WASI-compatible node.js target
    add_executable(nanochrono_wasi nanochrono_wasm.c)
    target_compile_definitions(nanochrono_wasi PRIVATE __wasm__=1 __wasi__=1)
    set_target_properties(nanochrono_wasi PROPERTIES
        SUFFIX ".wasm"
        LINK_FLAGS "-s WASM=1 -s STANDALONE_WASM=1 -s EXPORTED_FUNCTIONS='[${EXPORTED_LIST}]'")
elseif(WASI_SDK_PREFIX OR CMAKE_SYSTEM_NAME STREQUAL "WASI")
    add_executable(nanochrono_wasi nanochrono_wasm.c)
    target_compile_definitions(nanochrono_wasi PRIVATE __wasm__=1 __wasi__=1)
    set_target_properties(nanochrono_wasi PROPERTIES SUFFIX ".wasm")
else()
    message(STATUS "NanoChronometer WASM: no Emscripten/WASI toolchain detected — building stub only")
endif()
