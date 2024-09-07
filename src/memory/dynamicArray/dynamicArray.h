#pragma once

#include "defines.h"
#include <stdio.h>

typedef unsigned int u32;

typedef struct ArrayDescriptor_t ArrayDescriptor;
struct ArrayDescriptor_t {
    u32 size;
    u32 capacity;
    const u32 unitSize;
};

bool array_appendAndExpand(void **array, u32 *size, u32 *capacity, u32 unitSize, const void *appendData, u32 capacityIncrement);

// TODO: make a better array descriptor
bool array_appendDynamic(void **array, ArrayDescriptor *descriptor, const void *appendData, u32 capacityIncrement);
