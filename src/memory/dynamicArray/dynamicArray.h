#pragma once

#include "defines.h"
#include <stdio.h>

typedef unsigned int u32;

bool array_scale(void **array, size_t newSize);
bool array_appendDynamic(void **array, u32 *size, u32 *capacity, u32 unitSize, const void *appendData, u32 capacityIncrement);
