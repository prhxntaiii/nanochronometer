#!/usr/bin/env sh
set -eu

# Android target is library-only: libnanochrono.so + libnanochrono.a.
# No CLI, no GUI, no APK/Gradle target is generated here.

: "${ANDROID_NDK_HOME:=${ANDROID_NDK_ROOT:-}}"
if [ -z "${ANDROID_NDK_HOME}" ]; then
  echo "Set ANDROID_NDK_HOME or ANDROID_NDK_ROOT to your Android NDK path" >&2
  exit 1
fi

API="${NC_ANDROID_API:-24}"
BUILD_DIR="${BUILD_DIR:-build/android-arm64-lib-for-apk}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
LINKAGE="${NC_EXTERNAL_LINKAGE:-AUTO}"
NC_BUILD_SHARED="${NC_BUILD_SHARED:-ON}"
NC_BUILD_STATIC="${NC_BUILD_STATIC:-ON}"
NC_CRYPTO_BACKEND="${NC_CRYPTO_BACKEND:-AUTO}"

cmake -S . -B "${BUILD_DIR}" \
  -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM="android-${API}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DNC_TARGET_ARCH=arm64 \
  -DNC_TARGET_OS=android \
  -DNC_ANDROID_LIBRARY_ONLY=ON \
  -DNC_BUILD_SHARED="${NC_BUILD_SHARED}" \
  -DNC_BUILD_STATIC="${NC_BUILD_STATIC}" \
  -DNC_BUILD_CLI=OFF \
  -DNC_BUILD_GUI=OFF \
  -DNC_EXTERNAL_LINKAGE="${LINKAGE}" \
  -DNC_CRYPTO_BACKEND="${NC_CRYPTO_BACKEND}" \
  "$@"

cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}"

printf '\nAndroid library outputs:\n'
if [ "${NC_BUILD_SHARED}" = "ON" ]; then
  printf '  %s/dynamic/include/nanochrono.h\n' "${BUILD_DIR}"
  printf '  %s/dynamic/lib/libnanochrono.so\n' "${BUILD_DIR}"
fi
if [ "${NC_BUILD_STATIC}" = "ON" ]; then
  printf '  %s/static/include/nanochrono.h\n' "${BUILD_DIR}"
  printf '  %s/static/lib/libnanochrono.a\n' "${BUILD_DIR}"
fi
