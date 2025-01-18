#pragma once

#include "defines.h"
#include <stdlib.h>

typedef struct ArrayList_t
{
    // storage properties and storage
    struct
    {
        void *data;
        u32 capacity;
        u32 count;
    };
} ArrayList;

#define ARRAYLIST_COMPATIBLE_STRUCT(_type)                                                                             \
    struct                                                                                                             \
    {                                                                                                                  \
        _type *data;                                                                                                   \
        u32 capacity;                                                                                                  \
        u32 count;                                                                                                     \
    }

#define GENERIC_appendArrayList(_alist, _data) appendArrayList((ArrayList *)&(_alist), (void *)&(_data), sizeof(_data))

#define ARRAYLIST_FOREACH(_alist, _local) for (u32 _local = 0; _local < _alist.count; _local++)

#define APPEND_ARRAY_LIST(_alist, _data)                                                                               \
    ({                                                                                                                 \
        if (_alist.capacity <= _alist.count)                                                                           \
        {                                                                                                              \
            u32 localCapacity = (_alist.count + 1);                                                                    \
                                                                                                                       \
            void *tmpPtr = realloc(_alist.data, localCapacity * sizeof(_alist.data[0]));                               \
            if (tmpPtr == nullptr)                                                                                     \
                false; /* returns false (evaluates whole expression to false) */                                       \
            else                                                                                                       \
                _alist.data = tmpPtr;                                                                                  \
                                                                                                                       \
            _alist.capacity = localCapacity;                                                                           \
        }                                                                                                              \
                                                                                                                       \
        _alist.data[_alist.count++] = _data;                                                                           \
                                                                                                                       \
        true;                                                                                                          \
    })

ArrayList InitArrayList(u64 capacity, u32 unitSize);
bool appendArrayList(ArrayList *a, const void *restrict data, u32 unitSize);
void printIntArray(int *arr, int n);
