#pragma once

#include "defines.h"

bool arrayScale(void **array, u64 newSize);
bool appendArrayDynamic(void **array, const u32 unitSize, u32 *size, u32 *capacity, void *appendData);
void arrayPop(void *array, u32 unitSize, u32 *size, void *result);