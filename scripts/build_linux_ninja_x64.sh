#!/usr/bin/env sh
set -eu
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/nc_build_common.sh"
BUILD_DIR=${BUILD_DIR:-build_linux_ninja_x64}
BUILD_TYPE=${BUILD_TYPE:-Release}
NC_CRYPTO_BACKEND=${NC_CRYPTO_BACKEND:-AUTO}
NC_BUILD_SHARED=${NC_BUILD_SHARED:-ON}
NC_BUILD_STATIC=${NC_BUILD_STATIC:-ON}
nc_prepare_cmake_env linux x64
nc_print_target_summary
nc_prepare_build_dir "$BUILD_DIR"
# shellcheck disable=SC2086
cmake -S . -B "$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  $NC_CMAKE_SYSTEM_ARGS \
  $NC_CMAKE_COMPILER_ARGS \
  -DNC_TARGET_ARCH=x64 \
  -DNC_TARGET_OS=linux \
  -DNC_BUILD_SHARED="$NC_BUILD_SHARED" \
  -DNC_BUILD_STATIC="$NC_BUILD_STATIC" \
  -DNC_CRYPTO_BACKEND="$NC_CRYPTO_BACKEND"
nc_write_build_stamp "$BUILD_DIR"
cmake --build "$BUILD_DIR"
