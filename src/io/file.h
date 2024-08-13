#pragma once

#include <stdio.h>

// TODO: seperate file io header
typedef struct FileBuffer_t {
    char *data;
    size_t size;
} FileBuffer;

FileBuffer init_FileBuffer(FILE *path);
void free_FileBuffer(FileBuffer f);
