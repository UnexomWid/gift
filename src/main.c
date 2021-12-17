#ifndef __EMSCRIPTEN__
#    include <stdio.h>
#    include <stdlib.h>

#    include "gift.h"

#    define EXIT_OOM 137

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }

    FILE* file = fopen(argv[1], "rb");

    if (!file) {
        fprintf(stderr, "error: file does not exist or insufficient permissions\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(sizeof(char) * (size + 1));

    if (!buffer) {
        exit(EXIT_OOM);
    }

    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';
    fclose(file);

    char* result = gift(buffer);

    printf("%s\n", result);

    free(result);
}
#endif