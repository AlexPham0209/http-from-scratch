typedef enum Method {
    GET,
    POST,  
    PUT,
    DELETE,
} Method;

typedef struct Request {
    const Method method;
    const char* URI; 
} Request;

char* get_line(char** str);
Request* parse_request(char* request);
Method get_method(char* method);