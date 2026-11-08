#include "request.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
Request* parse_request(char* request) {
    // Parsing request line 
    char* curr = request; 
    char* request_line = consume(&curr);  
    printf("%s\n", request_line);

    // Parsing headers
    char* header = strsep(&curr, "\n");
    printf("%s\n", header);
    while (header != NULL && strlen(header) > 0) { 
        printf("%s\n", header); 
        header = strsep(&curr, "\n");
    }

    // Parsing body
    return NULL;
}

char* consume(char** str) {
    if (!str || !*str) return NULL;
   
    char* res = strsep(str, "\r");
    
    // Skipping \n token in line
    if (*str && **str == '\n') 
        (*str)++;
    
    return res;
}