#include "dynamicArray.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>

bool array_appendDynamic(void **array, u32 *size, u32 *capacity, u32 unitSize, const void *appendData, u32 capacityIncrement)
{
    // dont bother with pointers (it's dangerous you know)
    u32 localCapacity = *capacity;
    u32 localSize = *size;

    // resize if capacity insufficient
    if (localCapacity <= localSize)
    {
        localCapacity = (localSize + capacityIncrement);

        if (array_scale(array, localCapacity * unitSize) == false)
        {
            // we don't update anything since we failed
            return false;
        }
    }

    // append
    memcpy(((char *)(*array) + (unitSize * localSize)), appendData, unitSize);
    localSize += 1;

    // write results
    *size = localSize;
    *capacity = localCapacity;

    return true;
}

bool array_scale(void **array, size_t newSize)
{
    void *tmpPtr = realloc(*array, newSize);
    if (tmpPtr == nullptr)
    {
        return false;
    }
    else
    {
        *array = tmpPtr;
    }

    return true;
}

