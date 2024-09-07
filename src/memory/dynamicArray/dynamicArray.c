#include "dynamicArray.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>

bool array_scale(void **array, size_t newSize);

bool array_appendDynamic(void **array, ArrayDescriptor *d, const void *appendData, u32 capacityIncrement) {
    // resize if capacity insufficient
    if (d->capacity <= d->size)
    {
        if (capacityIncrement == 0) {
            return false;
        }

        if (array_scale(array, (d->capacity + capacityIncrement) * d->unitSize) == false) {
            // we don't update anything since we failed
            return false;
        }

        d->capacity += capacityIncrement;
    }

    // append
    memcpy(((char *)(*array) + (d->unitSize * d->size)), appendData, d->unitSize);
    d->size += 1;

    return true;
}

bool array_appendAndExpand(void **array, u32 *size, u32 *capacity, u32 unitSize, const void *appendData, u32 capacityIncrement)
{
    // dont bother with pointers (it's dangerous you know)
    u32 localCapacity = *capacity;
    u32 localSize = *size;

    // resize if capacity insufficient
    if (localCapacity <= localSize)
    {
        if (capacityIncrement == 0) {
            return false;
        }

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

