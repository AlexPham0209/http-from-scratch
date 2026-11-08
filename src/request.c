#include "request.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

Request* parse_request(char* request) {
    // Parsing request line
    char* req = strdup(request); 
    char* curr = req; 
    char* request_line = get_line(&curr);  

    // Getting request line information (Method, request target, HTTP Version) 
    char* method = strsep(&request_line, " ");
    char* request_target = strsep(&request_line, " ");
    char* version = strsep(&request_line, " ");

    // Parsing headers
    char* header = get_line(&curr); 
    while (header != NULL && strlen(header) > 0) { 
        header = get_line(&curr);
    }

    // Consume empty line
    get_line(&curr);

    // Parsing body
    char* body = strdup(curr);

    free(req);
    req = NULL;
    curr = NULL;
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