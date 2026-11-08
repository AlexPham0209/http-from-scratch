#include "server.h"
#include <stdlib.h>
#include "request.h"

#define IP_ADDRESS "127.0.0.1"
#define PORT 3000

int main(int argc, char *argv[]) {
    char str[1][100] = {"GET / HTTP/1.1\r\nKEY: VALUE\r\nKEY2: VALUE2\r\n\r\nBODY"};
    char* ptr = str[0];

    parse_request(ptr);
    // Server server = {
    //     PORT,
    //     IP_ADDRESS,
    //     1024
    // };

    // if (run(&server) == EXIT_FAILURE)
    //     return EXIT_FAILURE;

    // return EXIT_SUCCESS;
}