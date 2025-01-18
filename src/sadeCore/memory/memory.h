#pragma once

#include "defines.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ARRAY_IF_FAIL_DO(_size, _action)                                                                           \
    ({                                                                                                                 \
        void *_local = malloc(_size);                                                                                   \
        if (_local == nullptr)                                                                                         \
        {                                                                                                              \
            _action;                                                                                                    \
        }                                                                                                              \
        _local;                                                                                                        \
    })

#define REALLOC_ARRAY_IF_FAIL_DO(array, size, action)                                                                  \
    {                                                                                                                  \
        void *_local = realloc(array, size);                                                                           \
        if (_local == nullptr)                                                                                         \
        {                                                                                                              \
            action;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            array = _local;                                                                                            \
        }                                                                                                              \
    }
    