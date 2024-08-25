#include <string.h>

#include "dictionary.h"
#include "defines.h"
#include "memory/arrayDynamics.h"

bool dictionary_add(Dictionary *d, KeyValuePair data) {
    u32 localSize = d->size;
    u32 localCapacity = d->capacity;
    if (appendArrayDynamic((void **)d->keys, &localSize, &localCapacity, data.key, sizeof(d->keys[0]), 1) &&
        appendArrayDynamic(d->values, &d->size, &d->capacity, data.key, sizeof(d->values[0]), 1))
    {
        // both operations successful
        return true;
    }

    // if first one fails there is no change
    // 
    // if second one fails dictionary capacity and size remains same
    // we do not need to consider reverting the changes made to the first operation if second fails
    // because free operation does not need size to free a memory
    return false;
}

bool dictionary_write(Dictionary d, KeyValuePair data) {
    for (u32 i = 0; i < d.size; i++) {
        if (0 == strcmp(d.keys[i], data.key)) {
            // WARN: this operation only replaces the reference not the value
            d.values[i] = data.value;
        }
    }

    return false;
}
