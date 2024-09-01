#pragma once

#include "defines.h"

enum ResourceType {RESOURCE_TYPE_SHADER, RESOURCE_TYPE_TEXTURE};
typedef struct Resource_t Resource;
typedef struct ResourceManager_t ResourceManager;

struct Resource_t {
    char *name;
    char *path;
    enum ResourceType type;
};

typedef struct DynamicArray_t {
    u32 size;
    u32 unitSize;
    u32 capacity;
    void *data;
} DynamicArray;

struct ResourceManager_t {
    DynamicArray resources;
};


