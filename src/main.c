#include "server.h"
#include <stdlib.h>

#define IP_ADDRESS "127.0.0.1"
#define PORT 3000

Response* handle(Request* request) {
    // Sending a message back to the client
    Response* response = malloc(sizeof(Response));

    // Status line
    char* version = "HTTP/1.1"; 
    int status_code = 200;
    char* reason_phrase = "OK";
    
    // Response headers
    Headers* headers = malloc(sizeof(Headers));
    headers_init(headers, 3);
    headers_add(headers, (Header){"Server", "Apache"});
    headers_add(headers, (Header){"Content-Type", "application/json"});
    headers_add(headers, (Header){"Last-Modified", "Sun, 15 Nov 2026 9:45:00 EST"}); 

    // Body 
    char* body = "{message: \"This is the response wow\"}";
     
    // Converting response struct into string
    response_init(
        response,
        version,
        status_code,
        reason_phrase,
        headers,
        body
    );

    return response;
} 

int main(int argc, char *argv[]) {
    Server server = {
        PORT,
        IP_ADDRESS,
        1024,
        *handle,
    };

    if (server_listen(&server) == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}