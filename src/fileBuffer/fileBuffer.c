#include "fileBuffer.h"
#include <stdio.h>
#include <stdlib.h>

// NOTE: do not forget to delete FileBuffer data from heap
FileBuffer construct_fileBuffer(FILE *file) {
    FileBuffer fb = {0};

    // get file size
    // WARN: I am not checking errors for these 2 lines
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);

    // allocate memory for buffer
    char *buffer = malloc(sizeof(char) * (filesize + 1));
    if (!buffer) {
        perror("malloc");
        return fb;
    }

    // read file from beginning to end
    rewind(file);
    fread(buffer, sizeof(char), filesize, file);
    if (ferror(file)) {
        perror("fread");
        free(buffer);
        return fb;
    }
    buffer[filesize] = '\0';

    // if everything successful set file buffer and return it
    // NOTE: we initialize fb to zero at the start in case something goes wrong
    fb.data = buffer;
    fb.size = filesize + 1;

    return fb;
}

void destruct_fileBuffer(FileBuffer f) {
    free(f.data);
}
