#include "request.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int is_crlf(const char* str);
char* get_word(char* dest, char* src, const char delimiter, const size_t length);

Request* parse_request(char* str) {
    // Parsing method
    char* method = malloc(sizeof(char) * 20);  
    str = get_word(method, str, ' ', 20);

    // Parsing request-target
    char request_target[100] = {0};
    str = get_word(request_target, str, ' ', 20);
    
    // Parsing HTTP version
    char version[50] = {0};

    // Skipping over CRLF
    str += 2;
}

char* get_word(char* dest, char* src, const char delimiter, const size_t length) {
    // Copying characters from src string into dest string util we stumble across a space 
    size_t i = 0;
    while (i < length && src[i] != '\0' && src[i] != delimiter) { 
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    // Skipping delimiter and returning the offset to the next word
    while (src[i] == delimiter)
        i++;
    return src + i;
}

int get_header(char* key, char* value, const char* src, size_t length) {
    size_t i = 0;
    
    // Processing key until we reach :
    while (i < length && src[i] != ':') {
        key[i] = src[i];
        i++;
    }
    key[i] = '\0';

    // Skipping spaces
    while (src[i] == ' ')
        i++;
    
    // Processing value until we reach CRLF \r\n
    while (i < length && !is_crlf(src + i)) {
        value[i] = src[i];
        i++;
    }
    key[i] = '\0';

    // Skipping CRLF
    while (src[i] != '\r' && src[i] != '\n')
        i++;

    return i;
}

int is_crlf(const char* str) {
    return str[0] != '\0' && str[1] != '\0' && str[0] == '\r' && str[1] == '\n';
} 