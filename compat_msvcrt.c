#include <stdarg.h>
#include <stdio.h>

int _vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    return vsnprintf(str, size, format, ap);
}
