typedef struct Server {
    const int PORT;
    const char* ADDRESS;
    const int BUFFER_SIZE;
} Server;

int run(Server* server);