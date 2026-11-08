#include <stddef.h>

typedef struct Header {
    char* key;
    char* value;
} Header;

typedef struct Headers {
    size_t capacity;
    size_t size;
    Header* data;
} Headers;

void headers_init(Headers* headers, size_t initial_capacity);
void headers_add(Headers* headers, Header header);
void headers_free(Headers* headers);