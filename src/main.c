#include <stdio.h>
#include "server.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Defining file descriptor of socket
    ssize_t val_read;
    
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    
    // Establishing a TCP socket that utilizes IPV4 addresses
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Setting up address
    socklen_t address_length = sizeof(address);
    memset(&address, 0, address_length);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to IPV4 address
    if (bind(server_fd, (struct sockaddr*)&address, address_length) == -1) {
        perror("Address bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections 
    printf("Listening for incoming connections\n");
    if (listen(server_fd, 10) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // Given that a client has connected, we will accept the incoming client connection on the listening socket
    // Creates a new socket for communication with the client, while still keeping the old one for listening for new connections. 
    struct sockaddr_in client_address; 
    socklen_t client_address_length = sizeof(client_address);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_length);

    if (client_fd == -1) {
        perror("Accepting failed");
        exit(EXIT_FAILURE);
    }

    // Shows connection and closes both sockets
    printf("Client connected successfully!\n");
    close(client_fd);
    close(server_fd);
}