#pragma once

#include <stdio.h>

// TODO: seperate file io header
typedef struct FileBuffer_t {
    char *data;
    size_t size;
} FileBuffer;

FileBuffer construct_fileBuffer(FILE *file);
void destruct_fileBuffer(FileBuffer f);
