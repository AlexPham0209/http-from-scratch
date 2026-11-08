typedef enum Method {
    GET,
    POST,  
    PUT,
} Method;

typedef struct Request {
    const Method method;
    const char* URI; 
} Request;

char* consume(char** str);
Request* parse_request(char* request);