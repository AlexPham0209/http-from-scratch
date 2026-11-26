#pragma once
#include "header.h"

typedef struct Response {
    char* version;
    int status_code; 
    char* reason_phrase;
    Headers* headers;
    char* body;
} Response;

void response_init(Response* response, char* version, int status_code, char* reason_phrase, Headers* headers, char* body);
char* response_to_string(Response* response);
size_t calculate_response_length(Response* response);
void response_free(Response* response);
char* get_reason_phrase(int code);
Response* generate_404(); 