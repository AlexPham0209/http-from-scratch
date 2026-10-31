#include "server.h"
#include <stdlib.h>

#define IP_ADDRESS "127.0.0.1"
#define PORT 3000

int main(int argc, char *argv[]) {
    Server server = {
        PORT,
        IP_ADDRESS,
        1024
    };

    if (run(&server) == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}