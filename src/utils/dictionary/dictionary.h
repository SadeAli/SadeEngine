#pragma once

#include "defines.h"

typedef struct KeyValuePair_t {
    char *key;
    void *value;
} KeyValuePair;

typedef struct Dictionary_t {
    u32 size;
    u32 capacity;
    char **keys;
    void **values;
} Dictionary;

bool dictionary_add(Dictionary *d, KeyValuePair data);
bool dictionary_write(Dictionary d, KeyValuePair data);
