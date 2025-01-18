#include "defines.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

S_INLINE void freeVariadic(void *ptr1, ...)
{
    va_list args;
    void *vp;
    free(ptr1);

    va_start(args, ptr1);

    while ((vp = va_arg(args, void *)) != 0)
    {
        free(vp);
    }

    va_end(args);
}