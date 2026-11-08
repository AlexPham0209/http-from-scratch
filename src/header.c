#include <header.h>
#include <stdlib.h>


void headers_init(Headers* headers, size_t initial_capacity) {
    headers->data = malloc(initial_capacity * sizeof(Header));
    headers->size = 0;  
    headers->capacity = initial_capacity;
} 

void headers_add(Headers* headers, Header header) {
    if (headers->size >= headers->capacity) {
        headers->capacity *= 2;
        headers->data = realloc(headers->data, headers->capacity * sizeof(Header));
    }
    
    headers->data[headers->size++] = header;
} 

void headers_free(Headers* headers) {
    free(headers->data);
    headers->data = NULL;
    headers->size = 0;
    headers->capacity = 0;
}
