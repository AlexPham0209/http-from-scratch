typedef enum Method {
    GET,
    POST,  
    PUT,
} Method;

typedef struct Request {
    const Method method;
    const char* URI; 
} Request;

Request* parse_request(char* request);