#!/usr/bin/env sh
set -eu

# Android ARM64 CLI build helper.
#
# Two modes are supported:
#   1. Native Termux on an Android ARM64 device: use Termux clang directly.
#   2. Cross-compile from Linux/macOS: use Android NDK clang/toolchain.
#
# This is still only a native executable + libraries build. It does not build
# an APK, Gradle project, or Java/Kotlin wrapper.
#
# Outputs:
#   build/termux-arm64/static/bin/nanochrono_cli          native Termux
#   build/android-termux-cli-arm64/static/bin/nanochrono_cli  NDK cross build
#   build/*/dynamic/lib/libnanochrono.so
#   build/*/static/lib/libnanochrono.a
#
# Cross-compile examples:
#   ANDROID_NDK_HOME=/opt/android-ndk ./scripts/build_android_termux_cli.sh
#   ANDROID_NDK=/opt/android-ndk NC_ANDROID_API=24 ./scripts/build_termux_arm64_cli.sh
#
# Native Termux example:
#   pkg install clang cmake ninja make git
#   ./scripts/build_android_termux_cli.sh

BUILD_TYPE="${BUILD_TYPE:-Release}"
NC_BUILD_SHARED="${NC_BUILD_SHARED:-ON}"
NC_BUILD_STATIC="${NC_BUILD_STATIC:-ON}"
NC_CRYPTO_BACKEND="${NC_CRYPTO_BACKEND:-AUTO}"
NC_EXTERNAL_LINKAGE="${NC_EXTERNAL_LINKAGE:-AUTO}"
NC_TERMUX_INSTALL="${NC_TERMUX_INSTALL:-0}"
INSTALL_PREFIX="${INSTALL_PREFIX:-${PREFIX:-${HOME}/.local}}"
API="${NC_ANDROID_API:-24}"

host_os="$(uname -s 2>/dev/null || echo unknown)"
host_arch="$(uname -m 2>/dev/null || echo unknown)"

is_termux=0
case "${PREFIX:-}" in
  */com.termux/*|/data/data/com.termux/*) is_termux=1 ;;
esac
if [ -n "${TERMUX_VERSION:-}" ]; then
  is_termux=1
fi

if [ "${is_termux}" = "1" ]; then
  case "${host_arch}" in
    aarch64|arm64) ;;
    *)
      echo "error: Termux native CLI build requires ARM64/aarch64; host arch is ${host_arch}" >&2
      exit 1
      ;;
  esac

  BUILD_DIR="${BUILD_DIR:-build/termux-arm64}"
  echo "NanoChronometer Android CLI mode: native Termux ARM64"

  GENERATOR_ARGS=""
  if command -v ninja >/dev/null 2>&1; then
    GENERATOR_ARGS="-G Ninja"
  fi

  # shellcheck disable=SC2086
  cmake -S . -B "${BUILD_DIR}" ${GENERATOR_ARGS} \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
    -DNC_TARGET_ARCH=arm64 \
    -DNC_TARGET_OS=android \
    -DNC_ANDROID_LIBRARY_ONLY=OFF \
    -DNC_BUILD_CLI=ON \
    -DNC_BUILD_GUI=OFF \
    -DNC_BUILD_SHARED="${NC_BUILD_SHARED}" \
    -DNC_BUILD_STATIC="${NC_BUILD_STATIC}" \
    -DNC_CRYPTO_BACKEND="${NC_CRYPTO_BACKEND}" \
    -DNC_EXTERNAL_LINKAGE="${NC_EXTERNAL_LINKAGE}" \
    "$@"
else
  case "${host_os}" in
    Linux|Darwin) ;;
    *)
      echo "error: cross-compiling the Android CLI requires Linux/macOS host or native Termux" >&2
      exit 1
      ;;
  esac

  : "${ANDROID_NDK_HOME:=${ANDROID_NDK_ROOT:-${ANDROID_NDK:-}}}"
  if [ -z "${ANDROID_NDK_HOME}" ]; then
    if [ -d /opt/android-ndk ]; then
      ANDROID_NDK_HOME=/opt/android-ndk
    else
      echo "error: not running inside Termux; set ANDROID_NDK_HOME, ANDROID_NDK_ROOT, or ANDROID_NDK" >&2
      echo "example: ANDROID_NDK_HOME=/opt/android-ndk ./scripts/build_android_termux_cli.sh" >&2
      exit 1
    fi
  fi
  if [ ! -f "${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake" ]; then
    echo "error: Android NDK toolchain file not found under: ${ANDROID_NDK_HOME}" >&2
    exit 1
  fi

  case "${host_os}:${host_arch}" in
    Linux:*) host_tag=linux-x86_64 ;;
    Darwin:arm64|Darwin:aarch64) host_tag=darwin-arm64 ;;
    Darwin:*) host_tag=darwin-x86_64 ;;
  esac

  toolbin="${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/${host_tag}/bin"
  if [ ! -d "${toolbin}" ]; then
    echo "error: NDK LLVM toolchain bin dir not found: ${toolbin}" >&2
    exit 1
  fi

  # Export explicit cross tools for sub-builds and for easy debugging. CMake is
  # still driven through the official NDK toolchain file so the Android sysroot,
  # linker flags, API level, and ABI handling stay correct.
  export ANDROID_NDK_HOME
  export CC="${CC:-${toolbin}/aarch64-linux-android${API}-clang}"
  export CXX="${CXX:-${toolbin}/aarch64-linux-android${API}-clang++}"
  export AR="${AR:-${toolbin}/llvm-ar}"
  export RANLIB="${RANLIB:-${toolbin}/llvm-ranlib}"
  export STRIP="${STRIP:-${toolbin}/llvm-strip}"
  export LD="${LD:-${toolbin}/ld.lld}"

  for tool in "${CC}" "${CXX}" "${AR}" "${RANLIB}" "${STRIP}"; do
    if [ ! -x "${tool}" ]; then
      echo "error: required Android cross tool not executable: ${tool}" >&2
      exit 1
    fi
  done

  BUILD_DIR="${BUILD_DIR:-build/android-termux-cli-arm64}"
  echo "NanoChronometer Android CLI mode: NDK cross-compile ARM64"
  echo "  NDK: ${ANDROID_NDK_HOME}"
  echo "  API: android-${API}"
  echo "  CC:  ${CC}"
  echo "  CXX: ${CXX}"

  cmake -S . -B "${BUILD_DIR}" \
    -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM="android-${API}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
    -DNC_TARGET_ARCH=arm64 \
    -DNC_TARGET_OS=android \
    -DNC_ANDROID_LIBRARY_ONLY=OFF \
    -DNC_BUILD_CLI=ON \
    -DNC_BUILD_GUI=OFF \
    -DNC_BUILD_SHARED="${NC_BUILD_SHARED}" \
    -DNC_BUILD_STATIC="${NC_BUILD_STATIC}" \
    -DNC_CRYPTO_BACKEND="${NC_CRYPTO_BACKEND}" \
    -DNC_EXTERNAL_LINKAGE="${NC_EXTERNAL_LINKAGE}" \
    "$@"
fi

cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}"

if [ "${NC_TERMUX_INSTALL}" = "1" ]; then
  cmake --install "${BUILD_DIR}" --config "${BUILD_TYPE}"
fi

printf '\nAndroid CLI outputs:\n'
if [ "${NC_BUILD_STATIC}" = "ON" ] && [ -f "${BUILD_DIR}/static/bin/nanochrono_cli" ]; then
  printf '  CLI:      %s/static/bin/nanochrono_cli\n' "${BUILD_DIR}"
fi
if [ "${NC_BUILD_SHARED}" = "ON" ] && [ -f "${BUILD_DIR}/dynamic/lib/libnanochrono.so" ]; then
  printf '  Shared:   %s/dynamic/lib/libnanochrono.so\n' "${BUILD_DIR}"
fi
if [ "${NC_BUILD_STATIC}" = "ON" ] && [ -f "${BUILD_DIR}/static/lib/libnanochrono.a" ]; then
  printf '  Static:   %s/static/lib/libnanochrono.a\n' "${BUILD_DIR}"
fi
if [ "${NC_TERMUX_INSTALL}" = "1" ]; then
  printf '  Installed under: %s\n' "${INSTALL_PREFIX}"
fi
