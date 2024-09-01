#include "arrayDynamics.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>

bool appendArrayDynamic(void **array, u32 *size, u32 *capacity, const void *appendData, u32 unitSize, u32 capacityIncrement)
{
    // dont bother with pointers (it's dangerous you know)
    u32 localCapacity = *capacity;
    u32 localSize = *size;

    // resize if capacity insufficient
    if (localCapacity <= localSize)
    {
        localCapacity = (localSize + capacityIncrement);

        if (arrayScale(array, localCapacity * unitSize) == false)
        {
            // we don't update anything since we failed
            return false;
        }
    }

    // append
    memcpy((*array + (unitSize * localSize)), appendData, unitSize);
    localSize += 1;

    // write results
    *capacity = localCapacity;
    *size = localSize;

    return true;
}

bool arrayScale(void **array, u64 newSize)
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

