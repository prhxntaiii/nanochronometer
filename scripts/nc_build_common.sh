#!/usr/bin/env sh
# Common build helpers for NanoChronometer scripts.
# The scripts intentionally treat target_arch != host_arch as cross-compiling.

nc_host_os() {
  case "$(uname -s 2>/dev/null || printf unknown)" in
    Linux*) printf '%s\n' linux ;;
    Darwin*) printf '%s\n' macos ;;
    MINGW*|MSYS*|CYGWIN*) printf '%s\n' windows ;;
    *) printf '%s\n' unknown ;;
  esac
}

nc_normalize_arch() {
  case "$1" in
    x86_64|amd64|AMD64|x64|X64) printf '%s\n' x64 ;;
    aarch64|arm64|ARM64|Arm64) printf '%s\n' arm64 ;;
    *) printf '%s\n' "$1" ;;
  esac
}

nc_host_arch() {
  nc_normalize_arch "$(uname -m 2>/dev/null || printf unknown)"
}

nc_cmake_system_name() {
  case "$1" in
    linux) printf '%s\n' Linux ;;
    windows) printf '%s\n' Windows ;;
    *) printf '%s\n' "$1" ;;
  esac
}

nc_cmake_processor() {
  target_os=$1
  target_arch=$2
  case "$target_os:$target_arch" in
    windows:x64) printf '%s\n' x86_64 ;;
    windows:arm64) printf '%s\n' ARM64 ;;
    linux:x64) printf '%s\n' x86_64 ;;
    linux:arm64) printf '%s\n' aarch64 ;;
    *:x64) printf '%s\n' x86_64 ;;
    *:arm64) printf '%s\n' aarch64 ;;
    *) printf '%s\n' "$target_arch" ;;
  esac
}

nc_tool_prefix() {
  target_os=$1
  target_arch=$2
  case "$target_os:$target_arch" in
    linux:x64) printf '%s\n' x86_64-linux-gnu ;;
    linux:arm64) printf '%s\n' aarch64-linux-gnu ;;
    windows:x64) printf '%s\n' x86_64-w64-mingw32 ;;
    windows:arm64) printf '%s\n' aarch64-w64-mingw32 ;;
    *) printf '%s\n' "" ;;
  esac
}

nc_have_tool() {
  command -v "$1" >/dev/null 2>&1
}

nc_require_tool() {
  tool=$1
  hint=$2
  if ! nc_have_tool "$tool"; then
    printf 'error: required tool not found: %s\n' "$tool" >&2
    if [ -n "$hint" ]; then
      printf '%s\n' "$hint" >&2
    fi
    exit 127
  fi
}

nc_prepare_cmake_env() {
  NC_TARGET_OS=$1
  NC_TARGET_ARCH=$(nc_normalize_arch "$2")
  NC_HOST_OS=$(nc_host_os)
  NC_HOST_ARCH=$(nc_host_arch)
  NC_IS_CROSS=0

  if [ "$NC_TARGET_OS" != "$NC_HOST_OS" ] || [ "$NC_TARGET_ARCH" != "$NC_HOST_ARCH" ]; then
    NC_IS_CROSS=1
  fi

  NC_CMAKE_SYSTEM_ARGS=""
  NC_CMAKE_COMPILER_ARGS=""
  NC_CMAKE_RC_ARGS=""
  NC_TOOLCHAIN_FAMILY="native"

  if [ "$NC_IS_CROSS" = "1" ]; then
    NC_CMAKE_SYSTEM_ARGS="-DCMAKE_SYSTEM_NAME=$(nc_cmake_system_name "$NC_TARGET_OS") -DCMAKE_SYSTEM_PROCESSOR=$(nc_cmake_processor "$NC_TARGET_OS" "$NC_TARGET_ARCH") -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY"
  fi

  if [ "$NC_TARGET_OS" = "linux" ]; then
    NC_TOOLCHAIN_FAMILY="gnu"
    if [ "$NC_IS_CROSS" = "1" ]; then
      prefix=$(nc_tool_prefix "$NC_TARGET_OS" "$NC_TARGET_ARCH")
      CC=${CC:-${prefix}-gcc}
      nc_require_tool "$CC" "Install the cross compiler or override it, for example: CC=${prefix}-gcc $0"
      NC_CMAKE_COMPILER_ARGS="-DCMAKE_C_COMPILER=$CC -DCMAKE_ASM_COMPILER=$CC"
    elif [ -n "${CC:-}" ]; then
      nc_require_tool "$CC" "Set CC to a compiler available in PATH."
      NC_CMAKE_COMPILER_ARGS="-DCMAKE_C_COMPILER=$CC -DCMAKE_ASM_COMPILER=$CC"
    fi
  elif [ "$NC_TARGET_OS" = "windows" ]; then
    NC_TOOLCHAIN_FAMILY="mingw"
    prefix=$(nc_tool_prefix "$NC_TARGET_OS" "$NC_TARGET_ARCH")
    CC=${CC:-${prefix}-gcc}
    RC=${RC:-${prefix}-windres}
    nc_require_tool "$CC" "Install the MinGW cross compiler or override it, for example: CC=${prefix}-gcc $0"
    nc_require_tool "$RC" "Install windres for this target or override it, for example: RC=${prefix}-windres $0"
    NC_CMAKE_COMPILER_ARGS="-DCMAKE_C_COMPILER=$CC"
    NC_CMAKE_RC_ARGS="-DCMAKE_RC_COMPILER=$RC"
  fi

  export NC_TARGET_OS NC_TARGET_ARCH NC_HOST_OS NC_HOST_ARCH NC_IS_CROSS NC_TOOLCHAIN_FAMILY
  export NC_CMAKE_SYSTEM_ARGS NC_CMAKE_COMPILER_ARGS NC_CMAKE_RC_ARGS
  export CC RC
}

nc_print_target_summary() {
  printf 'NanoChronometer build target: os=%s arch=%s host_os=%s host_arch=%s cross=%s toolchain=%s\n' \
    "$NC_TARGET_OS" "$NC_TARGET_ARCH" "$NC_HOST_OS" "$NC_HOST_ARCH" "$NC_IS_CROSS" "${NC_TOOLCHAIN_FAMILY:-unknown}"
}

nc_target_signature() {
  printf 'target_os=%s\n' "$NC_TARGET_OS"
  printf 'target_arch=%s\n' "$NC_TARGET_ARCH"
  printf 'host_os=%s\n' "$NC_HOST_OS"
  printf 'host_arch=%s\n' "$NC_HOST_ARCH"
  printf 'cross=%s\n' "$NC_IS_CROSS"
  printf 'toolchain=%s\n' "${NC_TOOLCHAIN_FAMILY:-unknown}"
  printf 'cc=%s\n' "${CC:-}"
  printf 'rc=%s\n' "${RC:-}"
  printf 'cmake_system_args=%s\n' "$NC_CMAKE_SYSTEM_ARGS"
  printf 'cmake_compiler_args=%s\n' "$NC_CMAKE_COMPILER_ARGS"
  printf 'cmake_rc_args=%s\n' "$NC_CMAKE_RC_ARGS"
  printf 'arm64_asm_arch=%s\n' "${NC_ARM64_ASM_ARCH:-}"
  printf 'output_layout=static_dynamic_v2\n'
}

nc_path_exists_note() {
  path=$1
  if [ -e "$path" ]; then
    printf '  %s\n' "$path"
  else
    printf '  %s (expected)\n' "$path"
  fi
}

nc_is_on() {
  case "${1:-}" in
    ON|on|On|1|true|TRUE|yes|YES) return 0 ;;
    *) return 1 ;;
  esac
}

nc_print_outputs() {
  build_dir=$1
  exe_ext=""
  shared_lib="libnanochrono.so"
  static_lib="libnanochrono.a"
  import_lib=""

  if [ "${NC_TARGET_OS:-}" = "windows" ]; then
    exe_ext=".exe"
    shared_lib="nanochrono.dll"
    case "${NC_TOOLCHAIN_FAMILY:-mingw}" in
      msvc)
        static_lib="nanochrono.lib"
        import_lib="nanochrono.lib"
        ;;
      *)
        # MinGW/GNU keeps GNU archive naming: static .a plus .dll.a import lib.
        static_lib="libnanochrono.a"
        import_lib="libnanochrono.dll.a"
        ;;
    esac
  fi

  if nc_is_on "${NC_BUILD_STATIC:-ON}"; then
    exe_package="static"
  else
    exe_package="dynamic"
  fi

  printf '\noutputs:\n'
  if nc_is_on "${NC_BUILD_STATIC:-ON}"; then
    nc_path_exists_note "$build_dir/static/include/nanochrono.h"
    nc_path_exists_note "$build_dir/static/lib/$static_lib"
  fi
  if nc_is_on "${NC_BUILD_SHARED:-ON}"; then
    nc_path_exists_note "$build_dir/dynamic/include/nanochrono.h"
    nc_path_exists_note "$build_dir/dynamic/lib/$shared_lib"
    if [ -n "$import_lib" ]; then
      nc_path_exists_note "$build_dir/dynamic/lib/$import_lib"
    fi
  fi
  nc_path_exists_note "$build_dir/$exe_package/bin/nanochrono_cli$exe_ext"
  if [ "${NC_TARGET_OS:-}" = "windows" ] && nc_is_on "${NC_BUILD_GUI:-OFF}"; then
    nc_path_exists_note "$build_dir/$exe_package/bin/nanochrono_gui$exe_ext"
  fi
}

nc_prepare_build_dir() {
  build_dir=$1
  stamp="$build_dir/.nanochronometer-target"
  tmp_stamp="$build_dir/.nanochronometer-target.new"

  if [ "${NC_CLEAN:-0}" = "1" ]; then
    printf 'Cleaning %s because NC_CLEAN=1\n' "$build_dir"
    rm -rf "$build_dir"
  elif [ -f "$build_dir/CMakeCache.txt" ] && [ ! -f "$stamp" ]; then
    printf 'Cleaning %s because it was created before target stamping; this avoids stale x64/arm64 objects.\n' "$build_dir"
    rm -rf "$build_dir"
  elif [ -f "$stamp" ]; then
    mkdir -p "$build_dir"
    nc_target_signature > "$tmp_stamp"
    if ! cmp -s "$stamp" "$tmp_stamp"; then
      printf 'Cleaning %s because the target/compiler signature changed.\n' "$build_dir"
      rm -rf "$build_dir"
    else
      rm -f "$tmp_stamp"
    fi
  fi
}

nc_write_build_stamp() {
  build_dir=$1
  mkdir -p "$build_dir"
  nc_target_signature > "$build_dir/.nanochronometer-target"
}
