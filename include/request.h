#include <stddef.h>
#include <header.h>
typedef enum Method {
    GET,
    POST,  
    PUT,
    DELETE,
} Method;

typedef struct Request {
    char* data;
    Method method;
    Headers* headers;
    char* request_target; 
    char* version;
    char* body;
} Request;

char* get_line(char** str);
Request* parse_request(char* request);
Method get_method(char* method);