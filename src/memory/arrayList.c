#include "arrayList.h"
#include "arrayDynamics.h"
#include "defines.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>

ArrayList InitArrayList(u64 capacity, u32 unitSize)
{
    const ArrayList a = (ArrayList){
        .count = 0,
        .data = malloc(unitSize * capacity),
        .capacity = a.data == nullptr ? 0 : capacity,
    };

    return a;
}

bool appendArrayList(ArrayList *a, const void *restrict data, u32 unitSize)
{
    const u32 scaleByUnits = 1;
    u32 localCapacity = a->capacity;

    // if 
    if (localCapacity <= a->count)
    {
        localCapacity = (a->count + scaleByUnits);

        if (arrayScale(&a->data, localCapacity * unitSize) == false)
        {
            // we don't update anything since we failed
            return false;
        }
    }

    memcpy((a->data + (unitSize * a->count)), data, unitSize);
    a->count += 1;

    // write results
    a->capacity = localCapacity;

    return true;
}

void printIntArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d - ", arr[i]);
    }
}