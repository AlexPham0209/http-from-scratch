#pragma once

#include "request.h"
#include "response.h"
typedef struct Server {
    const int PORT;
    const char* ADDRESS;
    const int BUFFER_SIZE;
    Response* (*handle)(Request* request);
} Server;

int server_listen(Server* server);