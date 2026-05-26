#include <stdarg.h>
#include <stdio.h>

/*
 * Compatibility shim for old MSVC CRTs only.
 *
 * MinGW-w64 already provides _vsnprintf in libmsvcrt.a. Defining it here
 * creates a multiple-definition link failure when cross-building Windows
 * targets with x86_64-w64-mingw32-gcc / aarch64-w64-mingw32-gcc.
 *
 * Modern MSVC also provides a conforming enough vsnprintf, so this wrapper is
 * only needed for very old MSVC versions where vsnprintf was absent.
 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
int _vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    return vsnprintf(str, size, format, ap);
}
#endif

/* Keep translation unit non-empty for strict/archive toolchains. */
int nanochrono_compat_msvcrt_translation_unit_anchor(void) {
    return 0;
}
