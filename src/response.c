#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void response_init(Response* response, char* version, int status_code, char* reason_phrase, Headers* headers, char* body) {
    if (response == NULL) return;

    response->version = version ? strdup(version) : NULL;
    response->status_code = status_code;
    response->reason_phrase = reason_phrase ? strdup(reason_phrase) : NULL;
    response->headers = headers;
    response->body = body ? strdup(body) : NULL;
}

char* response_to_string(Response* response) {
    if (response == NULL)
        return NULL;

    size_t response_length = calculate_response_length(response) + 1;
    char* res = malloc(sizeof(char) * response_length); 
    if (res == NULL) 
        return NULL;
    
    size_t offset = 0;

    // Add status line
    offset += snprintf(res + offset, response_length - offset, "%s %d %s\r\n", 
        response->version ? response->version : NULL, 
        response->status_code, 
        response->reason_phrase ? response->reason_phrase : NULL);

    // Get headers 
    Headers* headers = response->headers;
    if (headers != NULL) {
        for (int i = 0; i < headers->size; ++i) {
            Header header = headers->data[i];
            offset += snprintf(res + offset, response_length - offset, "%s: %s\r\n", header.key, header.value);
        }
    }

    // Concatenate both the empty line and body payload
    offset += snprintf(res + offset, response_length - offset, "\r\n%s", response->body);
    
    // // Set null terminator \0
    res[response_length - 1] = '\0';
    
    return res;
}

size_t calculate_response_length(Response* response) {
    if (response == NULL)
        return 0;

    size_t status_line_size = snprintf(NULL, 0, "%s %d %s\r\n", 
        response->version, response->status_code, response->reason_phrase);
 
    // Get headers 
    size_t headers_size = 0;
    Headers* headers = response->headers;
    if (headers != NULL) {
        for (int i = 0; i < headers->size; ++i) {
            Header header = headers->data[i]; 
            size_t header_size = snprintf(NULL, 0, "%s: %s\r\n", 
                header.key, header.value);
            headers_size += header_size;
        }    
    }

    // Get body size
    size_t body_size = snprintf(NULL, 0, "\r\n%s", response->body);
 
    // The size of the response will be the sum of the status line, header, and body length plus the empty line (\r\n) 
    return status_line_size + headers_size + body_size;
}

void response_free(Response* response) {
    if (response == NULL)
        return;

    if (response->headers != NULL)
        headers_free(response->headers);

    if (response->version != NULL)
        free(response->version);

    if (response->reason_phrase != NULL)
        free(response->reason_phrase);

    if (response->body != NULL)
        free(response->body);
    
    free(response);
}