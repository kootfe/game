#include "errors.h"
#include <stdarg.h>
#include <stdio.h>

void errf(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    va_end(args);
}
