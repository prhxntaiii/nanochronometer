cmake_minimum_required(VERSION 3.20)
project(NanoChronometer VERSION 0.5.6 LANGUAGES C)

option(NC_BUILD_SHARED "Build shared library" ON)
option(NC_BUILD_STATIC "Build static library" ON)
option(NC_BUILD_GUI "Build Win32 GUI executable on Windows targets" OFF)
option(NC_BUILD_CLI "Build CLI executable when supported" ON)
option(NC_ANDROID_LIBRARY_ONLY "Android NDK-style build emits only libnanochrono.so/.a; set OFF for native Termux CLI builds" ON)
option(NC_PREFER_MINGW_STATIC_A "Prefer MinGW static .a archives from externals over import libs" ON)

set(NC_TARGET_ARCH "AUTO" CACHE STRING "Target CPU architecture: AUTO, x64, arm64")
set_property(CACHE NC_TARGET_ARCH PROPERTY STRINGS AUTO x64 arm64)
set(NC_TARGET_OS "AUTO" CACHE STRING "Target OS ABI: AUTO, windows, linux, android")
set_property(CACHE NC_TARGET_OS PROPERTY STRINGS AUTO windows linux android)
set(NC_ANDROID_API "24" CACHE STRING "Android API level used by helper scripts/toolchain builds")
set(NC_EXTERNAL_LINKAGE "AUTO" CACHE STRING "External crypto library linkage: AUTO, STATIC, SHARED")
set_property(CACHE NC_EXTERNAL_LINKAGE PROPERTY STRINGS AUTO STATIC SHARED)
set(NC_CRYPTO_BACKEND "AUTO" CACHE STRING "AUTO;OPENSSL;BORINGSSL;LIBSODIUM;BOTH;NONE")
set(NC_ARM64_ASM_ARCH "armv8-a+crypto" CACHE STRING "GNU/Clang -march value used for ARM64 assembly files with optional crypto instructions")
set(NC_ARM64_SVE_ASM_ARCH "armv8.5-a+sve+sve2+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SVE/SVE2 assembler files")
set(NC_ARM64_SME_ASM_ARCH "armv9.2-a+sme+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SME assembler files")
# SME2 file uses an explicit .inst encoding for ZERO {ZT0}; only SME/ZA mnemonics
# require assembler feature syntax, keeping older NDK/binutils usable when possible.
set(NC_ARM64_SME2_ASM_ARCH "armv9.2-a+sme+crypto" CACHE STRING "GNU/Clang -march value for ARM64 SME2 assembler files")

# Avoid GNUInstallDirs cross-compile warnings before CMake knows the target ABI.
# The project only needs stable install directory names.
set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "Install directory for executables")
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Install directory for libraries")
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Install directory for headers")

# Build tree package layout. Keep static and dynamic artifacts fully separated:
#   <build>/static/{bin,include,lib}
#   <build>/dynamic/{bin,include,lib}
# Library filenames stay native to the compiler toolchain:
#   MinGW/GNU: static/lib/libnanochrono.a and dynamic/lib/libnanochrono.dll.a
#   MSVC:      static/lib/nanochrono.lib and dynamic/lib/nanochrono.lib
# On Windows the shared runtime DLL is intentionally emitted to dynamic/lib,
# not to a top-level bin directory.
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

# CMAKE_SIZEOF_VOID_P can be empty during cross builds if CMake cannot run/link
# the probe executable. When NC_TARGET_ARCH is explicit, trust it and only fail
# when CMake actually measured a non-64-bit ABI.
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

# Android has two supported build shapes:
#   1. NDK/library integration: libnanochrono.so + libnanochrono.a, no APK/Gradle/CLI.
#   2. Native Termux CLI: set -DNC_ANDROID_LIBRARY_ONLY=OFF -DNC_BUILD_CLI=ON.
# GUI remains disabled on Android.
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
  # nanochrono_instruction.S contains optional ARMv8 Crypto extension opcodes
  # such as AESE, SHA256H, and PMULL. Cross compilers often default to plain
  # armv8-a, whose assembler rejects those mnemonics. Enable only the ARM64
  # assembly sources, not the whole C project, so runtime feature detection
  # still decides whether those routines are executed.
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

add_library(nc_crypto_backend INTERFACE)
set(NC_EXTERNAL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/externals/${NC_ARCH}/${NC_ASM_PLATFORM}")
set(NC_LIBSODIUM_ROOT "${NC_EXTERNAL_ROOT}/libsodium")
if(NC_ARCH STREQUAL "arm64")
  set(NC_SSL_ROOT "${NC_EXTERNAL_ROOT}/boringssl")
else()
  set(NC_SSL_ROOT "${NC_EXTERNAL_ROOT}/openssl")
endif()

function(nc_pick_lib OUT ROOT NAME)
  set(_paths "${ROOT}/lib" "${ROOT}/lib/${ANDROID_ABI}" "${ROOT}/lib64" "${ROOT}")
  unset(_picked)
  string(TOUPPER "${NC_EXTERNAL_LINKAGE}" _nc_external_linkage)

  if(_nc_external_linkage STREQUAL "STATIC")
    foreach(_candidate "lib${NAME}.a" "${NAME}.a")
      foreach(_dir ${_paths})
        if(EXISTS "${_dir}/${_candidate}")
          set(_picked "${_dir}/${_candidate}")
          break()
        endif()
      endforeach()
      if(_picked)
        break()
      endif()
    endforeach()
  elseif(_nc_external_linkage STREQUAL "SHARED")
    foreach(_candidate "lib${NAME}.so" "${NAME}.so" "${NAME}.dll" "lib${NAME}.dylib")
      foreach(_dir ${_paths})
        if(EXISTS "${_dir}/${_candidate}")
          set(_picked "${_dir}/${_candidate}")
          break()
        endif()
      endforeach()
      if(_picked)
        break()
      endif()
    endforeach()
  endif()

  if(NOT _picked AND MINGW AND NC_PREFER_MINGW_STATIC_A)
    foreach(_candidate "lib${NAME}.a" "${NAME}.a")
      foreach(_dir ${_paths})
        if(EXISTS "${_dir}/${_candidate}")
          set(_picked "${_dir}/${_candidate}")
          break()
        endif()
      endforeach()
      if(_picked)
        break()
      endif()
    endforeach()
  elseif(NOT _picked AND MSVC)
    foreach(_candidate "${NAME}.lib" "lib${NAME}.lib")
      foreach(_dir ${_paths})
        if(EXISTS "${_dir}/${_candidate}")
          set(_picked "${_dir}/${_candidate}")
          break()
        endif()
      endforeach()
      if(_picked)
        break()
      endif()
    endforeach()
  endif()

  if(NOT _picked)
    set(_find_var "${OUT}_FOUND")
    unset(${_find_var} CACHE)
    if(_nc_external_linkage STREQUAL "STATIC")
      find_library(${_find_var} NAMES lib${NAME}.a ${NAME}.a PATHS ${_paths} NO_DEFAULT_PATH)
    elseif(_nc_external_linkage STREQUAL "SHARED")
      find_library(${_find_var} NAMES ${NAME} lib${NAME}.so ${NAME}.so PATHS ${_paths} NO_DEFAULT_PATH)
    else()
      find_library(${_find_var} NAMES ${NAME} lib${NAME} ${NAME}.lib lib${NAME}.a lib${NAME}.so PATHS ${_paths} NO_DEFAULT_PATH)
    endif()
    if(NOT "${${_find_var}}" STREQUAL "" AND NOT "${${_find_var}}" MATCHES "-NOTFOUND$")
      set(_picked "${${_find_var}}")
    endif()
  endif()

  set(${OUT} "${_picked}" PARENT_SCOPE)
endfunction()

string(TOUPPER "${NC_CRYPTO_BACKEND}" NC_CRYPTO_BACKEND)
set(NC_FOUND_OPENSSL 0)
set(NC_FOUND_LIBSODIUM 0)
set(NC_CRYPTO_BACKEND_NAME "unlinked")

if(NOT NC_CRYPTO_BACKEND STREQUAL "NONE")
  nc_pick_lib(NC_CRYPTO_LIB "${NC_SSL_ROOT}" crypto)
  nc_pick_lib(NC_SODIUM_LIB "${NC_LIBSODIUM_ROOT}" sodium)

  if(NC_CRYPTO_LIB AND EXISTS "${NC_SSL_ROOT}/include/openssl/evp.h")
    set(NC_FOUND_OPENSSL 1)
  endif()
  if(NC_SODIUM_LIB AND EXISTS "${NC_LIBSODIUM_ROOT}/include/sodium.h")
    set(NC_FOUND_LIBSODIUM 1)
  endif()

  if(NC_CRYPTO_BACKEND STREQUAL "AUTO")
    if(NC_FOUND_OPENSSL AND NC_FOUND_LIBSODIUM)
      set(NC_CRYPTO_BACKEND BOTH)
    elseif(NC_FOUND_OPENSSL)
      if(NC_ARCH STREQUAL "arm64")
        set(NC_CRYPTO_BACKEND BORINGSSL)
      else()
        set(NC_CRYPTO_BACKEND OPENSSL)
      endif()
    elseif(NC_FOUND_LIBSODIUM)
      set(NC_CRYPTO_BACKEND LIBSODIUM)
    else()
      set(NC_CRYPTO_BACKEND NONE)
    endif()
  endif()

  if(NC_CRYPTO_BACKEND MATCHES "OPENSSL|BORINGSSL|BOTH")
    if(NOT NC_FOUND_OPENSSL)
      message(FATAL_ERROR
        "NC_CRYPTO_BACKEND=${NC_CRYPTO_BACKEND} needs libcrypto plus headers at ${NC_SSL_ROOT}. "
        "Expected ${NC_SSL_ROOT}/lib/libcrypto.a or crypto.lib and ${NC_SSL_ROOT}/include/openssl/evp.h")
    endif()
    target_include_directories(nc_crypto_backend INTERFACE "${NC_SSL_ROOT}/include")
    target_link_libraries(nc_crypto_backend INTERFACE "${NC_CRYPTO_LIB}")
    # BoringSSL libcrypto.a contains C++ object files. With static archives,
    # libstdc++ must appear after libcrypto.a in the final link line.
    if((NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android") AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
      target_link_libraries(nc_crypto_backend INTERFACE stdc++)
    endif()
  endif()

  if(NC_CRYPTO_BACKEND MATCHES "LIBSODIUM|BOTH")
    if(NOT NC_FOUND_LIBSODIUM)
      message(FATAL_ERROR
        "NC_CRYPTO_BACKEND=${NC_CRYPTO_BACKEND} needs libsodium plus headers at ${NC_LIBSODIUM_ROOT}. "
        "Expected ${NC_LIBSODIUM_ROOT}/lib/libsodium.a or sodium.lib and ${NC_LIBSODIUM_ROOT}/include/sodium.h")
    endif()
    target_include_directories(nc_crypto_backend INTERFACE "${NC_LIBSODIUM_ROOT}/include")
    target_link_libraries(nc_crypto_backend INTERFACE "${NC_SODIUM_LIB}")
    target_compile_definitions(nc_crypto_backend INTERFACE SODIUM_STATIC=1)
  endif()

  if(NC_CRYPTO_BACKEND STREQUAL "OPENSSL")
    set(NC_CRYPTO_BACKEND_NAME "openssl")
    target_compile_definitions(nc_crypto_backend INTERFACE NC_USE_OPENSSL=1)
  elseif(NC_CRYPTO_BACKEND STREQUAL "BORINGSSL")
    set(NC_CRYPTO_BACKEND_NAME "boringssl")
    target_compile_definitions(nc_crypto_backend INTERFACE NC_USE_BORINGSSL=1)
  elseif(NC_CRYPTO_BACKEND STREQUAL "LIBSODIUM")
    set(NC_CRYPTO_BACKEND_NAME "libsodium")
    target_compile_definitions(nc_crypto_backend INTERFACE NC_USE_LIBSODIUM=1)
  elseif(NC_CRYPTO_BACKEND STREQUAL "BOTH")
    set(NC_CRYPTO_BACKEND_NAME "openssl+libsodium")
    target_compile_definitions(nc_crypto_backend INTERFACE NC_USE_OPENSSL=1 NC_USE_LIBSODIUM=1)
  endif()

  if(NOT NC_CRYPTO_BACKEND STREQUAL "NONE")
    target_compile_definitions(nc_crypto_backend INTERFACE "NC_CRYPTO_BACKEND_NAME=\"${NC_CRYPTO_BACKEND_NAME}\"")
  endif()

  if(NC_OS STREQUAL "android" AND NC_CRYPTO_BACKEND MATCHES "OPENSSL|BORINGSSL|LIBSODIUM|BOTH")
    target_link_libraries(nc_crypto_backend INTERFACE log)
  endif()

  if(NC_OS STREQUAL "windows" AND NC_CRYPTO_BACKEND MATCHES "OPENSSL|BORINGSSL|LIBSODIUM|BOTH")
    # Static OpenSSL/libsodium archives do not encode their Win32 system
    # dependencies. Keep them after the crypto .a/.lib files in link order.
    target_link_libraries(nc_crypto_backend INTERFACE ws2_32 crypt32 bcrypt advapi32 user32)
  endif()
endif()

message(STATUS "NanoChronometer arch=${NC_ARCH} platform=${NC_ASM_PLATFORM} crypto=${NC_CRYPTO_BACKEND} external_linkage=${NC_EXTERNAL_LINKAGE} mingw_static_a=${NC_PREFER_MINGW_STATIC_A}")
message(STATUS "NanoChronometer external root=${NC_EXTERNAL_ROOT}")
if(NC_ARCH STREQUAL "arm64")
  message(STATUS "NanoChronometer arm64 asm march=${NC_ARM64_ASM_ARCH}")
endif()
message(STATUS "NanoChronometer libcrypto=${NC_CRYPTO_LIB}")
message(STATUS "NanoChronometer libsodium=${NC_SODIUM_LIB}")

# Linux cannot link non-PIC static crypto archives into a shared object:
#   relocation R_X86_64_PC32 ... can not be used when making a shared object
# The bundled Linux externals are static .a archives, so keep crypto enabled for
# the static library / CLI and build libnanochrono.so without those backends.
set(NC_SHARED_LINKS_CRYPTO 1)
if((NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android") AND NC_BUILD_SHARED AND NOT NC_CRYPTO_BACKEND STREQUAL "NONE")
  set(_nc_shared_static_crypto 0)
  if(NC_CRYPTO_BACKEND MATCHES "OPENSSL|BORINGSSL|BOTH" AND NC_CRYPTO_LIB MATCHES "[.]a$")
    set(_nc_shared_static_crypto 1)
  endif()
  if(NC_CRYPTO_BACKEND MATCHES "LIBSODIUM|BOTH" AND NC_SODIUM_LIB MATCHES "[.]a$")
    set(_nc_shared_static_crypto 1)
  endif()
  if(_nc_shared_static_crypto)
    set(NC_SHARED_LINKS_CRYPTO 0)
    message(WARNING "Static ${NC_OS} crypto archives may be unsafe to link into libnanochrono.so unless built with PIC; shared library will be built without crypto backends. The static library and CLI still link ${NC_CRYPTO_BACKEND}. Use -DNC_EXTERNAL_LINKAGE=SHARED with .so crypto libs, or rebuild externals with -fPIC if libnanochrono.so needs crypto.")
  endif()
endif()

set(NC_CORE_SOURCES src/nanochrono_runtime.c src/nanochrono_instruction.c src/nanochrono_sidechannel.c src/platform/android/nanochrono_android.c compat_msvcrt.c ${NC_ASM_SOURCES})
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
  target_include_directories(nanochrono PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  if(NC_SHARED_LINKS_CRYPTO)
    target_link_libraries(nanochrono PRIVATE nc_crypto_backend)
  endif()
  if(NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android")
    target_link_libraries(nanochrono PRIVATE m)
  elseif(NC_OS STREQUAL "windows")
    # nanochrono_sidechannel.c uses Winsock APIs for NTP queries.
    # Link ws2_32 explicitly even when crypto backends are disabled.
    target_link_libraries(nanochrono PRIVATE ws2_32)
  endif()
endif()
if(NC_BUILD_STATIC)
  nc_copy_public_header("${NC_STATIC_INCLUDE_DIR}")
  add_library(nanochrono_static STATIC ${NC_CORE_SOURCES})
  target_include_directories(nanochrono_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(nanochrono_static PUBLIC nc_crypto_backend)
  if(NC_OS STREQUAL "linux" OR NC_OS STREQUAL "android")
    target_link_libraries(nanochrono_static PUBLIC m)
  elseif(NC_OS STREQUAL "windows")
    # Public because executables linking the static library must also link Winsock.
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
  add_executable(nanochrono_cli nanochrono_cli.c compat_msvcrt.c)
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
  if(NC_ARCH STREQUAL "arm64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main_arm64.c")
    set(NC_GUI_MAIN_SOURCE main_arm64.c)
  elseif(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main_x64.c")
    set(NC_GUI_MAIN_SOURCE main_x64.c)
  else()
    set(NC_GUI_MAIN_SOURCE main.c)
  endif()
  set(NC_GUI_SOURCES ${NC_GUI_MAIN_SOURCE} app.rc bench_kernels.c)
  if(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bench_kernels_avx2.c")
    list(APPEND NC_GUI_SOURCES bench_kernels_avx2.c)
  endif()
  # GUI-only legacy benchmark ASM symbols: nc_bench_kernel_asm_legacy and nc_bench_kernel_mmx.
  if(NC_ARCH STREQUAL "x64" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/asm/x64/windows/bench_kernels_asm.asm")
    list(APPEND NC_GUI_SOURCES asm/x64/windows/bench_kernels_asm.asm)
  endif()
  add_executable(nanochrono_gui WIN32 compat_msvcrt.c ${NC_GUI_SOURCES})
  target_include_directories(nanochrono_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  if(TARGET nanochrono_static)
    target_link_libraries(nanochrono_gui PRIVATE nanochrono_static)
  else()
    target_link_libraries(nanochrono_gui PRIVATE nanochrono)
  endif()
  # The GUI calls OpenSSL EVP/libsodium directly for benchmark modes 2 and 3,
  # so link the crypto backend here too. This propagates include paths,
  # NC_USE_* compile definitions, and final link libraries into main.c.
  target_link_libraries(nanochrono_gui PRIVATE nc_crypto_backend)
  target_link_libraries(nanochrono_gui PRIVATE user32 gdi32 dwmapi winmm msimg32 comdlg32 comctl32 shell32 ws2_32)
  set_target_properties(nanochrono_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${NC_EXE_OUTPUT_DIR}"
    OUTPUT_NAME nanochrono_gui)
  if(MINGW)
    target_compile_definitions(nanochrono_gui PRIVATE NC_GUI_MINGW=1)
  endif()
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND NC_ARCH STREQUAL "x64")
    # GUI benchmark sources use x86 intrinsics directly. Keep them isolated from
    # the core library and compile them with the exact feature flags required by
    # the intrinsics they contain. This fixes MinGW GCC errors such as:
    #   _mm_sha256rnds2_epu32: target specific option mismatch  -> needs -msha
    #   _xgetbv: target specific option mismatch                -> needs -mxsave
    set_source_files_properties(${NC_GUI_MAIN_SOURCE} PROPERTIES
      COMPILE_OPTIONS "-mxsave;-Wno-unknown-pragmas")
    set_source_files_properties(bench_kernels.c PROPERTIES
      COMPILE_OPTIONS "-msse4.2;-maes;-mpclmul;-msha;-mxsave")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bench_kernels_avx2.c")
      set_source_files_properties(bench_kernels_avx2.c PROPERTIES COMPILE_OPTIONS "-mavx2;-mfma;-mf16c;-mxsave")
    endif()
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set_source_files_properties(${NC_GUI_MAIN_SOURCE} PROPERTIES COMPILE_OPTIONS "-Wno-unknown-pragmas")
  elseif(MSVC AND NC_ARCH STREQUAL "x64")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bench_kernels_avx2.c")
      set_source_files_properties(bench_kernels_avx2.c PROPERTIES COMPILE_OPTIONS "/arch:AVX2")
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
