#include "server.h"
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define false 0
#define true 1

int run(Server* server) {
    // Defining file descriptor of socket
    struct sockaddr_in address;
    
    // Establishing a TCP socket that utilizes IPV4 addresses
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }
    
    // Sets up IPV4 address
    socklen_t address_length = sizeof(address);
    memset(&address, 0, address_length);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server->ADDRESS);
    address.sin_port = htons(server->PORT);

    // Binding socket to IPV4 address
    if (bind(server_fd, (struct sockaddr*)&address, address_length) == -1) {
        perror("Address bind failed");
        return EXIT_FAILURE;
    }

    // Listening for connections, only accepting 10 connections at a time in its backlog
    if (listen(server_fd, 10) == -1) {
        perror("Error listening on socket");
        return EXIT_FAILURE;
    }

    // Accepts client connection, reads their message, and sends a response back; Afterwards, it closes the client socket connection
    while (true) {
        struct sockaddr_in client_address; 
        socklen_t client_address_length = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_length);

        if (client_fd == -1) {
            perror("Accepting failed");
            continue;
        }

        printf("Client connection from: %s\n", inet_ntoa(client_address.sin_addr));

        // Reading and printing message from client connection
        int BUFFER_SIZE = server->BUFFER_SIZE;
        char buffer[BUFFER_SIZE];
        int bytes_received = recvfrom(client_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &client_address_length);
        buffer[bytes_received] = '\0';
        printf("Message: %s", buffer);

        // Sending a message back to the client
        const char* message = "Hello back";
        size_t message_length = strlen(message);
        printf("Sending: %s\n\n", message);
        send(client_fd, message, message_length, 0);
        
        // Closing client socket connection
        close(client_fd);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}