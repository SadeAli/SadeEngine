#include "arrayDynamics.h"
#include "defines.h"
#include "memory.h"
#include <string.h>

bool appendArrayDynamic(void **array, const u32 unitSize, u32 *size, u32 *capacity, void *appendData)
{
    const u32 scaleByUnits = 1;

    // dont bother with pointers (it's dangerous you know)
    u32 localCapacity = *capacity;
    u32 localSize = *size;

    // resize if capacity insufficient
    if (localCapacity <= localSize)
    {
        localCapacity = (localSize + scaleByUnits);

        if (arrayScale(array, localCapacity * unitSize) == false)
        {
            // we don't update anything since we failed
            return false;
        }
    }

    // append
    memcpy((*array + (unitSize * localSize++)), appendData, unitSize);

    // write results
    *capacity = localCapacity;
    *size = localSize;

    return true;
}

void arrayPop(void *array, u32 unitSize, u32 *size, void *result)
{
    (*size) -= 1;
    memcpy(result, array + (unitSize * (*size)), unitSize);
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

