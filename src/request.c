#include "request.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

Request* parse_request(char* request) {
    // Invalid request if there is no empty line
    if (!strstr(request, "\r\n\r\n"))
        return NULL;

    // Duplicating request string (heap-allocated)
    char* data = strdup(request); 
    char* curr = data;

    // Getting request line information (Method, request target, HTTP Version) 
    char* request_line = get_line(&curr); 
    if (curr == NULL) {
        free(data);
        return NULL;
    }
    
    // Parsing method string 
    char* method_str = strsep(&request_line, " ");
    if (request_line == NULL) {
        free(data);
        return NULL;
    }

    // Converting method string into Method enum
    Method method = get_method(method_str);
    if (method == INVALID) {
        free(data);
        return NULL; 
    } 

    // Parsing request target or end point 
    char* request_target = strsep(&request_line, " ");
    if (request_line == NULL) {
        free(data);
        return NULL; 
    }

    // Parsing version
    char* version = strsep(&request_line, " ");
    if (strlen(version) == 0 || request_line != NULL) {
        free(data);
        return NULL; 
    }

    // Initializing dynamically growing list of headers with a starting capacity of 5 
    Headers* headers = malloc(sizeof(Headers));
    headers_init(headers, 5);
    char* header = get_line(&curr);

    // Parsing headers
    while (header != NULL && strlen(header) > 0) { 
        char* key = strsep(&header, ":"); 
        if (header == NULL) {
            free(data);
            headers_free(headers);
            return NULL;
        }
        
        // Skip over whitespace header
        char* value = header + 1;
        
        // Creating header object and added it to dynamic headers list
        Header h = {key, value};
        headers_add(headers, h);
        header = get_line(&curr);
    }

    // Parsing body
    char* body = curr;
    
    // Setting request object
    Request* req = malloc(sizeof(Request));
    req->data = data;
    req->headers = headers;
    req->request_target = request_target;
    req->method = method;
    req->version = version;
    req->body = body;

    return req;
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
    return INVALID;
}

void request_free(Request* request) {
    headers_free(request->headers);
    free(request->data);
    free(request);
}

char* get_line(char** str) {
    if (!str || !*str) 
        return NULL;
    
    char* res = strsep(str, "\r");
    // Skipping \n token in line
    if (*str && **str == '\n') 
        (*str)++;
    
    return res;
}