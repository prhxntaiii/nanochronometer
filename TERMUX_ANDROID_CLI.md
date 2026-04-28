# NanoChronometer Android Termux CLI

This build path creates a native Android ARM64 command-line executable for
Termux. It is not an APK, does not use Gradle, and does not use the Android NDK
CMake toolchain file. It builds directly inside Termux with the Termux clang,
libc, linker and package environment.

## Dependencies

```sh
pkg install clang cmake ninja make git
```

Optional crypto dependencies depend on how the repository's `externals` folder
is populated. To build the chronometer/profiler core without crypto backends:

```sh
NC_CRYPTO_BACKEND=NONE ./scripts/build_termux_arm64_cli.sh
```

## Build

```sh
./scripts/build_termux_arm64_cli.sh
```

Equivalent alias:

```sh
./scripts/build_android_termux_cli.sh
```

Default outputs:

```text
build/termux-arm64/static/bin/nanochrono_cli
build/termux-arm64/dynamic/lib/libnanochrono.so
build/termux-arm64/static/lib/libnanochrono.a
```

## Install into Termux prefix

```sh
NC_TERMUX_INSTALL=1 ./scripts/build_termux_arm64_cli.sh
```

This installs under `$PREFIX` when Termux defines it, otherwise under
`$HOME/.local`.

## Notes

- Android NDK builds remain library-only by default via `NC_ANDROID_LIBRARY_ONLY=ON`.
- Termux CLI builds explicitly use `NC_ANDROID_LIBRARY_ONLY=OFF`.
- Root/Magisk/SuperSU is not requested during normal CLI build. Privileged
  profiling backends must request consent at runtime before using `su`.
