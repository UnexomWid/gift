#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "gift.h"

#define BUFFER_INITIAL_SIZE 16
#define EXIT_OOM 137

typedef struct {
    char*  data;
    size_t size;
    size_t cap;
} CharBuffer;

static void buffer_init(CharBuffer* buffer) {
    buffer->data = NULL;
    buffer->size = 0;
    buffer->cap  = 0;
}

static void buffer_grow(CharBuffer* buffer, size_t amount) {
    if (!buffer->data) {
        buffer->data = malloc(sizeof(char) * (buffer->cap = BUFFER_INITIAL_SIZE));

        if (!buffer->data) {
            exit(EXIT_OOM);
        }
    }

    while (buffer->size + amount > buffer->cap) {
        buffer->data = realloc(buffer->data, sizeof(char) * (buffer->cap <<= 1));

        if (!buffer->data) {
            exit(EXIT_OOM);
        }
    }
}

static void buffer_append_char(CharBuffer* buffer, char c) {
    buffer_grow(buffer, sizeof(c));
    buffer->data[buffer->size++] = c;
}

static void buffer_append_str(CharBuffer* buffer, const char* src, size_t length) {
    buffer_grow(buffer, length);
    memcpy(buffer->data + buffer->size, src, sizeof(char) * length);
    buffer->size += length;
}

static void buffer_clear(CharBuffer* buffer) {
    if (buffer->data) {
        free(buffer->data);
    }

    buffer_init(buffer);
}

static void buffer_end(CharBuffer* buffer) {
    buffer_append_char(buffer, '\0');
}

static void buffer_coal(CharBuffer* buffer) {
    buffer_clear(buffer);
    buffer_append_str(buffer, "coal", sizeof("coal") - 1);
    buffer_end(buffer);
}

static bool coal(uint32_t hash_result) {
    return (hash_result & 3) != 0;
}

static uint32_t hash_init() {
    return 2166136261u;
}

// FNV-1a
static uint32_t hash_step(uint32_t hash, char c) {
    return (hash ^ c) * 16777619;
}

static char to_lower(char c) {
    return (char) (c + ((c - 'A' <= 'Z' - 'A') << 5));
}

char* gift(const char* src) {
    char input;

    CharBuffer output;
    char       current_char = '\0';
    uint32_t   current_hash = hash_init();

    buffer_init(&output);

    while ((input = *src++) != '\0') {
        switch (input) {
        case ' ':
        case '\t':
            continue;
        case '\n':
        case '\r':
            if (current_char != '\0') {
                if (coal(current_hash)) {
                    buffer_coal(&output);
                    return output.data;
                } else {
                    buffer_append_char(&output, current_char);
                }

                current_char = '\0';
                current_hash = hash_init();
            }
            break;
        default:
            if (current_char == '\0') {
                current_char = input;
            }

            current_hash = hash_step(current_hash, to_lower(input));
            break;
        }
    }

    if (current_char != '\0') {
        if (coal(current_hash)) {
            buffer_coal(&output);
            return output.data;
        } else {
            buffer_append_char(&output, current_char);
        }
    } else if (output.size == 0) {
        buffer_coal(&output);
        return output.data;
    }

    buffer_end(&output);
    return output.data;
}

void gift_free_result(char* result) {
    free(result);
}