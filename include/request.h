#include <stddef.h>
#include <header.h>
typedef enum Method {
    GET,
    POST,  
    PUT,
    DELETE,
    INVALID,
} Method;

typedef struct Request {
    char* data;
    Method method;
    char* request_target; 
    char* version;
    Headers* headers;
    char* body;
} Request;

char* get_line(char** str);
Request* parse_request(char* request);
void request_free(Request* request);
Method get_method(char* method);