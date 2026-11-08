#include "request.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

Request* parse_request(char* request) {
    // Parsing request line
    char* req = strdup(request); 
    char* curr = req; 
    char* request_line = get_line(&curr);  

    // Getting request line information (Method, request target, HTTP Version) 
    char* method = strsep(&request_line, " ");
    char* request_target = strsep(&request_line, " ");
    char* version = strsep(&request_line, " ");

    printf("%s %s %s\n", method, request_target, version);

    // Initializing dynamically growing list of headers with a starting capacity of 5 
    Headers* headers = malloc(sizeof(Headers));
    headers_init(headers, 5);
    char* header = get_line(&curr);

    // Parsing headers
    while (header != NULL && strlen(header) > 0) { 
        char* key = strsep(&header, ":");
        char* value = header + 1;
        
        Header h = {key, value};
        headers_add(headers, h);
        header = get_line(&curr);
    }

    // Parsing body
    char* body = strdup(curr);
    printf("%s\n", body);
    return NULL;
}

Method get_method(char* method) {
    if (strcmp(method, "GET") == 0)    
        return GET;
    else if (strcmp(method, "POST") == 0)
        return POST;
    else if (strcmp(method, "PUT") == 0)
        return PUT;
    else if (strcmp(method, "DELETE") == 0)
        return DELETE;     
    return GET;
}

char* get_line(char** str) {
    if (!str || !*str) return NULL;
    
    char* res = strsep(str, "\r");
    // Skipping \n token in line
    if (*str && **str == '\n') 
        (*str)++;
    
    return res;
}