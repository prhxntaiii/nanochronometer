cmake_minimum_required(VERSION 3.20)
project(NanoChronometer VERSION 0.5.6 LANGUAGES C)

option(NC_BUILD_SHARED "Build shared library" ON)
option(NC_BUILD_STATIC "Build static library" ON)
option(NC_BUILD_GUI "Build Win32 GUI executable on Windows targets" OFF)
option(NC_BUILD_CLI "Build CLI executable when supported" ON)
option(NC_ANDROID_LIBRARY_ONLY "Android NDK-style build emits only libnanochrono.so/.a; set OFF for native Termux CLI builds" ON)
option(NC_LINK_STATIC "Link libcrypto from system as a static archive (.a) instead of the default shared library" OFF)

set(NC_TARGET_ARCH "AUTO" CACHE STRING "Target CPU architecture: AUTO, x64, arm64")
set_property(CACHE NC_TARGET_ARCH PROPERTY STRINGS AUTO x64 arm64)
set(NC_TARGET_OS "AUTO" CACHE STRING "Target OS ABI: AUTO, windows, linux, android")
set_property(CACHE NC_TARGET_OS PROPERTY STRINGS AUTO windows linux android)
set(NC_ANDROID_API "24" CACHE STRING "Android API level used by helper scripts/toolchain builds")
set(NC_CRYPTO_BACKEND "AUTO" CACHE STRING "Crypto backend selection: AUTO, OPENSSL, NONE")
set_property(CACHE NC_CRYPTO_BACKEND PROPERTY STRINGS AUTO OPENSSL NONE)
set(NC_ARM64_ASM_ARCH "armv8-a+crypto" CACHE STRING "GNU/Clang -march value used for ARM64 assembly files with optional crypto instructions")
set(NC_ARM64_SVE_ASM_ARCH "armv8.5-a+sve+sve2+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SVE/SVE2 assembler files")
set(NC_ARM64_SME_ASM_ARCH "armv9.2-a+sme+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SME assembler files")
set(NC_ARM64_SME2_ASM_ARCH "armv9.2-a+sme+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SME2 assembler files")

# Avoid GNUInstallDirs cross-compile warnings before CMake knows the target ABI.
set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "Install directory for executables")
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Install directory for libraries")
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Install directory for headers")

# Build tree package layout.
set(NC_STATIC_ROOT "${CMAKE_BINARY_DIR}/static")
set(NC_DYNAMIC_ROOT "${CMAKE_BINARY_DIR}/dynamic")
set(NC_STATIC_BIN_DIR "${NC_STATIC_ROOT}/bin")
set(NC_STATIC_INCLUDE_DIR "${NC_STATIC_ROOT}/include")
set(NC_STATIC_LIB_DIR "${NC_STATIC_ROOT}/lib")
set(NC_DYNAMIC_BIN_DIR "${NC_DYNAMIC_ROOT}/bin")
set(NC_DYNAMIC_INCLUDE_DIR "${NC_DYNAMIC_ROOT}/include")
set(NC_DYNAMIC_LIB_DIR "${NC_DYNAMIC_ROOT}/lib")

function(nc_copy_public_header OUT_DIR)
  file(MAKE_DIRECTORY "${OUT_DIR}")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/nanochrono.h" "${OUT_DIR}/nanochrono.h" COPYONLY)
endfunction()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(
    $<$<COMPILE_LANGUAGE:C>:/W3>
    $<$<COMPILE_LANGUAGE:C>:/O2>
    $<$<COMPILE_LANGUAGE:C>:/utf-8>)
else()
  add_compile_options(
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-O2>)
endif()

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" NC_PROC)
string(TOLOWER "${NC_TARGET_ARCH}" NC_TARGET_ARCH_LOWER)
if(NC_TARGET_ARCH_LOWER STREQUAL "auto" OR NC_TARGET_ARCH_LOWER STREQUAL "")
  if(NC_PROC MATCHES "x86_64|amd64|x64")
    set(NC_ARCH x64)
  elseif(NC_PROC MATCHES "aarch64|arm64")
    set(NC_ARCH arm64)
  else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}. Set -DNC_TARGET_ARCH=x64 or arm64.")
  endif()
elseif(NC_TARGET_ARCH_LOWER MATCHES "x64|x86_64|amd64")
  set(NC_ARCH x64)
elseif(NC_TARGET_ARCH_LOWER MATCHES "arm64|aarch64")
  set(NC_ARCH arm64)
else()
  message(FATAL_ERROR "Unsupported NC_TARGET_ARCH=${NC_TARGET_ARCH}; use AUTO, x64, or arm64")
endif()

if(DEFINED CMAKE_SIZEOF_VOID_P AND CMAKE_SIZEOF_VOID_P AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "NanoChronometer requires a 64-bit target")
endif()

string(TOLOWER "${NC_TARGET_OS}" NC_TARGET_OS_LOWER)
string(TOLOWER "${CMAKE_SYSTEM_NAME}" NC_SYSTEM_NAME_LOWER)
if(NC_TARGET_OS_LOWER STREQUAL "auto" OR NC_TARGET_OS_LOWER STREQUAL "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR WIN32 OR MINGW OR MSYS)
    set(NC_OS windows)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Android" OR ANDROID)
    set(NC_OS android)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR UNIX)
    set(NC_OS linux)
  else()
    message(FATAL_ERROR "Unsupported target OS: ${CMAKE_SYSTEM_NAME}. Set -DNC_TARGET_OS=windows, linux, or android.")
  endif()
elseif(NC_TARGET_OS_LOWER MATCHES "win|windows|mingw|msvc")
  set(NC_OS windows)
elseif(NC_TARGET_OS_LOWER MATCHES "linux")
  set(NC_OS linux)
elseif(NC_TARGET_OS_LOWER MATCHES "android|ndk")
  set(NC_OS android)
else()
  message(FATAL_ERROR "Unsupported NC_TARGET_OS=${NC_TARGET_OS}; use AUTO, windows, linux, or android")
endif()

if(NC_OS STREQUAL "android")
  if(NC_ANDROID_LIBRARY_ONLY)
    if(NC_BUILD_CLI)
      message(STATUS "Android library-only target selected: disabling nanochrono_cli")
    endif()
    set(NC_BUILD_CLI OFF CACHE BOOL "Build CLI executable when supported" FORCE)
  else()
    message(STATUS "Android native/Termux target selected: CLI build allowed")
  endif()
  if(NC_BUILD_GUI)
    message(STATUS "Android target selected: disabling nanochrono_gui")
  endif()
  set(NC_BUILD_GUI OFF CACHE BOOL "Build Win32 GUI executable on Windows targets" FORCE)
endif()

set(NC_ASM_PLATFORM ${NC_OS})

if(NC_ARCH STREQUAL "x64")
  enable_language(ASM_NASM)
  set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Wall -O2")
  if(NC_OS STREQUAL "windows")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)
  else()
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
  endif()
  set(NC_ASM_SOURCES
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_legacy.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_avx.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_mmx.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_sse.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_sse2.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_sse3.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_ssse3.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_sse41.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_sse42.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_f16c.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_fma.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_avx2.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_avx_vnni.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_avx512.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_avx512_vnni.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_instruction.asm
    asm/x64/${NC_ASM_PLATFORM}/nanochrono_microbench_generated.asm)
elseif(NC_ARCH STREQUAL "arm64")
  enable_language(ASM)
  set(NC_ASM_SOURCES
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_scalar.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_neon.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sve.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sve2.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sme.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sme2.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_instruction.S
    asm/arm64/${NC_ASM_PLATFORM}/nanochrono_microbench_generated.S)
  if(NC_ARM64_ASM_ARCH AND CMAKE_ASM_COMPILER_ID MATCHES "GNU|Clang")
    set_source_files_properties(${NC_ASM_SOURCES} PROPERTIES
      COMPILE_OPTIONS "-march=${NC_ARM64_ASM_ARCH}")
    if(NC_ARM64_SVE_ASM_ARCH)
      set_source_files_properties(
        asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sve.S
        asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sve2.S
        PROPERTIES COMPILE_OPTIONS "-march=${NC_ARM64_SVE_ASM_ARCH}")
    endif()
    if(NC_ARM64_SME_ASM_ARCH)
      set_source_files_properties(
        asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sme.S
        PROPERTIES COMPILE_OPTIONS "-march=${NC_ARM64_SME_ASM_ARCH}")
    endif()
    if(NC_ARM64_SME2_ASM_ARCH)
      set_source_files_properties(
        asm/arm64/${NC_ASM_PLATFORM}/nanochrono_arm64_sme2.S
        PROPERTIES COMPILE_OPTIONS "-march=${NC_ARM64_SME2_ASM_ARCH}")
    endif()
  endif()
else()
  message(FATAL_ERROR "Unsupported architecture: ${NC_ARCH}")
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Crypto backend: system OpenSSL only (no externals/ directory).
# The EVP shim (src/nc_evp_shim.c) resolves symbols at runtime via dlsym so
# that missing EVP symbols degrade gracefully to "N/A" rather than hard-failing.
# NC_LINK_STATIC=ON finds libcrypto.a from the system for the static library.
# ─────────────────────────────────────────────────────────────────────────────
add_library(nc_crypto_backend INTERFACE)
string(TOUPPER "${NC_CRYPTO_BACKEND}" NC_CRYPTO_BACKEND)

if(NOT NC_CRYPTO_BACKEND STREQUAL "NONE")
  if(NC_OS STREQUAL "android")
    # Android NDK bundles its own libcrypto via BoringSSL in the NDK sysroot.
    # The EVP shim uses dlopen at runtime; no compile-time linkage needed here.
    message(STATUS "NanoChronometer Android: EVP shim uses runtime dlopen for libcrypto")
    set(NC_CRYPTO_BACKEND OPENSSL)
  else()
    if(NC_LINK_STATIC)
      # Search for a system static archive (libcrypto.a).
      find_library(NC_SYSTEM_CRYPTO_STATIC
        NAMES libcrypto.a crypto
        PATH_SUFFIXES lib lib64
        DOC "System static libcrypto archive")
      if(NC_SYSTEM_CRYPTO_STATIC AND NC_SYSTEM_CRYPTO_STATIC MATCHES "[.]a$")
        message(STATUS "NanoChronometer static libcrypto: ${NC_SYSTEM_CRYPTO_STATIC}")
        target_link_libraries(nc_crypto_backend INTERFACE "${NC_SYSTEM_CRYPTO_STATIC}")
        if(NC_OS STREQUAL "linux" AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
          # Static OpenSSL may contain C++ objects (engine/provider layer).
          target_link_libraries(nc_crypto_backend INTERFACE stdc++ dl pthread)
        endif()
      else()
        message(WARNING "NC_LINK_STATIC=ON but no libcrypto.a found on system; falling back to shared libcrypto via EVP shim")
        set(NC_LINK_STATIC OFF CACHE BOOL "" FORCE)
      endif()
    endif()

    if(NOT NC_LINK_STATIC)
      # Default: dynamic link via the EVP shim (dlopen at runtime).
      # The shim handles missing symbols gracefully (N/A).
      if(NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android")
        target_link_libraries(nc_crypto_backend INTERFACE dl)
      endif()
    endif()

    # Find headers for compile-time type information only (no library path gating).
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND OR OPENSSL_FOUND)
      target_include_directories(nc_crypto_backend INTERFACE "${OPENSSL_INCLUDE_DIR}")
      message(STATUS "NanoChronometer OpenSSL headers: ${OPENSSL_INCLUDE_DIR}")
    else()
      # Try pkg-config as fallback for cross-compile environments.
      find_package(PkgConfig QUIET)
      if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_OPENSSL QUIET openssl)
        if(PC_OPENSSL_FOUND)
          target_include_directories(nc_crypto_backend INTERFACE ${PC_OPENSSL_INCLUDE_DIRS})
        endif()
      endif()
    endif()

    set(NC_CRYPTO_BACKEND OPENSSL)
  endif()

  target_compile_definitions(nc_crypto_backend INTERFACE NC_USE_OPENSSL=1)
  target_compile_definitions(nc_crypto_backend INTERFACE "NC_CRYPTO_BACKEND_NAME=\"openssl-shim\"")

  if(NC_OS STREQUAL "android")
    target_link_libraries(nc_crypto_backend INTERFACE log)
  endif()

  if(NC_OS STREQUAL "windows")
    # Winsock for NTP queries and Windows crypto runtime deps.
    target_link_libraries(nc_crypto_backend INTERFACE ws2_32 crypt32 bcrypt advapi32)
  endif()
endif()

if(NC_CRYPTO_BACKEND STREQUAL "NONE")
  target_compile_definitions(nc_crypto_backend INTERFACE "NC_CRYPTO_BACKEND_NAME=\"none\"")
endif()

message(STATUS "NanoChronometer arch=${NC_ARCH} platform=${NC_ASM_PLATFORM} crypto=${NC_CRYPTO_BACKEND} static_link=${NC_LINK_STATIC}")

# On Linux with dynamic EVP shim the shared library can always link crypto
# because the shim calls dlopen at runtime (no static .a in the shared object).
set(NC_SHARED_LINKS_CRYPTO 1)

set(NC_CORE_SOURCES
  src/nanochrono_runtime.c
  src/nanochrono_instruction.c
  src/nanochrono_sidechannel.c
  src/nc_evp_shim.c
  src/nc_hw_dispatch.c
  src/platform/android/nanochrono_android.c
  src/platform/windows/compat_msvcrt.c
  ${NC_ASM_SOURCES})
if(NC_ARCH STREQUAL "arm64" AND NC_OS STREQUAL "windows")
  list(APPEND NC_CORE_SOURCES src/nanochrono_arm64_x86_compat.c)
endif()

if(NC_BUILD_SHARED)
  nc_copy_public_header("${NC_DYNAMIC_INCLUDE_DIR}")
  add_library(nanochrono SHARED ${NC_CORE_SOURCES})
  set_target_properties(nanochrono PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${NC_DYNAMIC_LIB_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${NC_DYNAMIC_LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${NC_DYNAMIC_LIB_DIR}")
  target_compile_definitions(nanochrono PRIVATE NANOCHRONO_EXPORTS)
  target_include_directories(nanochrono PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
  if(NC_SHARED_LINKS_CRYPTO)
    target_link_libraries(nanochrono PRIVATE nc_crypto_backend)
  endif()
  if(NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android")
    target_link_libraries(nanochrono PRIVATE m)
  elseif(NC_OS STREQUAL "windows")
    target_link_libraries(nanochrono PRIVATE ws2_32)
  endif()
endif()
if(NC_BUILD_STATIC)
  nc_copy_public_header("${NC_STATIC_INCLUDE_DIR}")
  add_library(nanochrono_static STATIC ${NC_CORE_SOURCES})
  target_include_directories(nanochrono_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(nanochrono_static PUBLIC nc_crypto_backend)
  if(NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android")
    target_link_libraries(nanochrono_static PUBLIC m)
  elseif(NC_OS STREQUAL "windows")
    target_link_libraries(nanochrono_static PUBLIC ws2_32)
  endif()
  set_target_properties(nanochrono_static PROPERTIES
    OUTPUT_NAME nanochrono
    ARCHIVE_OUTPUT_DIRECTORY "${NC_STATIC_LIB_DIR}")
endif()

if(TARGET nanochrono_static)
  set(NC_EXE_OUTPUT_DIR "${NC_STATIC_BIN_DIR}")
else()
  set(NC_EXE_OUTPUT_DIR "${NC_DYNAMIC_BIN_DIR}")
endif()

if(NC_BUILD_CLI)
  add_executable(nanochrono_cli cli/nanochrono_cli.c src/platform/windows/compat_msvcrt.c)
  set_target_properties(nanochrono_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NC_EXE_OUTPUT_DIR}")
  target_include_directories(nanochrono_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  if(TARGET nanochrono_static)
    target_link_libraries(nanochrono_cli PRIVATE nanochrono_static)
  else()
    target_link_libraries(nanochrono_cli PRIVATE nanochrono)
  endif()
endif()
if(NC_OS STREQUAL "windows" AND NC_BUILD_GUI)
  enable_language(RC)
  if(NC_ARCH STREQUAL "arm64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui/main_arm64.c")
    set(NC_GUI_MAIN_SOURCE gui/main_arm64.c)
  elseif(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui/main_x64.c")
    set(NC_GUI_MAIN_SOURCE gui/main_x64.c)
  else()
    set(NC_GUI_MAIN_SOURCE gui/main.c)
  endif()
  set(NC_GUI_SOURCES ${NC_GUI_MAIN_SOURCE} gui/app.rc gui/bench_kernels.c)
  if(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui/bench_kernels_avx2.c")
    list(APPEND NC_GUI_SOURCES gui/bench_kernels_avx2.c)
  endif()
  if(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/asm/x64/windows/bench_kernels_asm.asm")
    list(APPEND NC_GUI_SOURCES asm/x64/windows/bench_kernels_asm.asm)
  endif()
  add_executable(nanochrono_gui WIN32 src/platform/windows/compat_msvcrt.c ${NC_GUI_SOURCES})
  target_include_directories(nanochrono_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gui)
  if(TARGET nanochrono_static)
    target_link_libraries(nanochrono_gui PRIVATE nanochrono_static)
  else()
    target_link_libraries(nanochrono_gui PRIVATE nanochrono)
  endif()
  target_link_libraries(nanochrono_gui PRIVATE nc_crypto_backend)
  target_link_libraries(nanochrono_gui PRIVATE user32 gdi32 dwmapi winmm msimg32 comdlg32 comctl32 shell32 ws2_32)
  set_target_properties(nanochrono_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${NC_EXE_OUTPUT_DIR}"
    OUTPUT_NAME nanochrono_gui)
  if(MINGW)
    target_compile_definitions(nanochrono_gui PRIVATE NC_GUI_MINGW=1)
  endif()
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND NC_ARCH STREQUAL "x64")
    set_source_files_properties(${NC_GUI_MAIN_SOURCE} PROPERTIES
      COMPILE_OPTIONS "-mxsave;-Wno-unknown-pragmas")
    set_source_files_properties(gui/bench_kernels.c PROPERTIES
      COMPILE_OPTIONS "-msse4.2;-maes;-mpclmul;-msha;-mxsave")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui/bench_kernels_avx2.c")
      set_source_files_properties(gui/bench_kernels_avx2.c PROPERTIES COMPILE_OPTIONS "-mavx2;-mfma;-mf16c;-mxsave")
    endif()
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set_source_files_properties(${NC_GUI_MAIN_SOURCE} PROPERTIES COMPILE_OPTIONS "-Wno-unknown-pragmas")
  elseif(MSVC AND NC_ARCH STREQUAL "x64")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gui/bench_kernels_avx2.c")
      set_source_files_properties(gui/bench_kernels_avx2.c PROPERTIES COMPILE_OPTIONS "/arch:AVX2")
    endif()
  endif()
endif()
if(TARGET nanochrono)
  install(FILES nanochrono.h DESTINATION dynamic/include)
  install(TARGETS nanochrono
    RUNTIME DESTINATION dynamic/lib
    LIBRARY DESTINATION dynamic/lib
    ARCHIVE DESTINATION dynamic/lib)
endif()
if(TARGET nanochrono_static)
  install(FILES nanochrono.h DESTINATION static/include)
  install(TARGETS nanochrono_static ARCHIVE DESTINATION static/lib)
endif()
if(TARGET nanochrono_cli)
  if(TARGET nanochrono_static)
    install(TARGETS nanochrono_cli RUNTIME DESTINATION static/bin)
  else()
    install(TARGETS nanochrono_cli RUNTIME DESTINATION dynamic/bin)
  endif()
endif()
if(TARGET nanochrono_gui)
  if(TARGET nanochrono_static)
    install(TARGETS nanochrono_gui RUNTIME DESTINATION static/bin)
  else()
    install(TARGETS nanochrono_gui RUNTIME DESTINATION dynamic/bin)
  endif()
endif()
