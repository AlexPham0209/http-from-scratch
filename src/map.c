#include "map.h"
#include <string.h>

// Hashing string using FNV-1 hashing algorithm 
long long hash(const char* key) {
    long long hash = FNV_OFFSET_BASIS;
    size_t length = strlen(key);
    for (int i = 0; i < length; ++i) {
        hash *= FNV_PRIME;
        hash ^= key[i];
    }
    return hash;
}