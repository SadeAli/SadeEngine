#pragma once

#include "defines.h"

bool arrayScale(void **array, u64 newSize);
// bool appendArrayDynamic(void **array, const u32 unitSize, u32 *size, u32 *capacity, void *appendData);
bool appendArrayDynamic(void **array, u32 *size, u32 *capacity, void const *appendData, u32 unitSize, u32 capacityIncrement);
