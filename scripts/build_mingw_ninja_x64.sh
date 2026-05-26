#!/usr/bin/env sh
set -eu
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/nc_build_common.sh"
BUILD_DIR=${BUILD_DIR:-build_mingw_ninja_x64}
BUILD_TYPE=${BUILD_TYPE:-Release}
NC_CRYPTO_BACKEND=${NC_CRYPTO_BACKEND:-AUTO}
NC_BUILD_SHARED=${NC_BUILD_SHARED:-ON}
NC_BUILD_STATIC=${NC_BUILD_STATIC:-ON}
NC_BUILD_GUI=${NC_BUILD_GUI:-ON}
nc_prepare_cmake_env windows x64
nc_print_target_summary
nc_prepare_build_dir "$BUILD_DIR"
# shellcheck disable=SC2086
cmake -S . -B "$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  $NC_CMAKE_SYSTEM_ARGS \
  $NC_CMAKE_COMPILER_ARGS \
  $NC_CMAKE_RC_ARGS \
  -DNC_TARGET_ARCH=x64 \
  -DNC_TARGET_OS=windows \
  -DNC_PREFER_MINGW_STATIC_A=ON \
  -DNC_CRYPTO_BACKEND="$NC_CRYPTO_BACKEND" \
  -DNC_BUILD_SHARED="$NC_BUILD_SHARED" \
  -DNC_BUILD_STATIC="$NC_BUILD_STATIC" \
  -DNC_BUILD_GUI="$NC_BUILD_GUI"
nc_write_build_stamp "$BUILD_DIR"
cmake --build "$BUILD_DIR"
printf '\noutputs:\n'
printf '  %s/bin/nanochrono_gui.exe\n' "$BUILD_DIR"
printf '  %s/bin/nanochrono_cli.exe\n' "$BUILD_DIR"
printf '  %s/bin/nanochrono.dll\n' "$BUILD_DIR"
printf '  %s/static/libnanochrono.a\n' "$BUILD_DIR"
