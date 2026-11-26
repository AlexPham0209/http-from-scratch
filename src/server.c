#include "server.h"
#include "request.h"
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <response.h>

#define false 0
#define true 1

typedef struct ConnectionInfo {
    int client_fd;
    int* connections; 
    pthread_mutex_t* connection_mutex;
    struct sockaddr_in* address; 
    Server* server;
} ConnectionInfo;

void* handle_connection(void* ptr);
void close_connection(int fd, Request* request, Response* response, char* message, ConnectionInfo* connection_info); 

int server_listen(Server* server) {
    // Establishing a TCP socket that utilizes IPV4 addresses
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }
    
    // Sets up IPV4 address
    struct sockaddr_in address = {0};
    socklen_t address_length = sizeof(address);
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

    // Initialize connection count along with its mutex lock
    int connections = 0;
    pthread_mutex_t connection_mutex;
    pthread_mutex_init(&connection_mutex, NULL);
    
    // Accepts client connection, reads their message, and sends a response back; Afterwards, it closes the client socket connection
    while (true) {
        struct sockaddr_in* client_address = malloc(sizeof(struct sockaddr_in)); 
        socklen_t client_address_length = sizeof(*client_address);
        int client_fd = accept(server_fd, (struct sockaddr*)client_address, &client_address_length);
        if (client_fd == -1) {
            perror("Accepting failed");
            continue;
        }
        
        // Spawning a single thread per connection; Yes, I know that this is bad practice but it works for now
        // Planning on switching to thread pool, forking, or I/O multiplexing
        pthread_t thread;
        ConnectionInfo* connection_info = malloc(sizeof(ConnectionInfo));
        connection_info->client_fd = client_fd;
        connection_info->connections = &connections;
        connection_info->connection_mutex = &connection_mutex;
        connection_info->address = client_address;
        connection_info->server = server;
        int ret = pthread_create(&thread, NULL, &handle_connection, (void*)connection_info);
        if (ret == -1)
            perror("Error from pthread");    
    }

    close(server_fd);
    return EXIT_SUCCESS;
}

void* handle_connection(void* ptr) {
    ConnectionInfo* connection_info = (ConnectionInfo*) ptr;
    int fd = connection_info->client_fd;
    int* connections = connection_info->connections;
    pthread_mutex_t* connection_mutex = connection_info->connection_mutex;
    struct sockaddr_in* address = connection_info->address;
    Server* server = connection_info->server; 
     
    printf("Client connection from: %s\n", inet_ntoa(address->sin_addr));

    // Increment the connection count
    pthread_mutex_lock(connection_mutex);
    (*connections)++;
    printf("Connection Count: %d\n", *connections);
    pthread_mutex_unlock(connection_mutex);

    // Reading and printing message from client connection
    socklen_t address_length = sizeof(*address);
    int BUFFER_SIZE = server->BUFFER_SIZE;
    char buffer[BUFFER_SIZE];
    int bytes_received = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)address, &address_length);
    buffer[bytes_received] = '\0';
    printf("Message: %s", buffer);

    // Parsing the request and creating a request object allocated to the heap
    Request* request = request_parse(buffer);
    if (request == NULL) {
        perror("Invalid HTTP request");
        close_connection(fd, NULL, NULL, NULL, connection_info);
        return NULL;
    }

    // Use handler function given to server to process and create response
    Response* response = server->handle(request);
    if (response == NULL) {
        perror("Invalid response parsing");
        close_connection(fd, request, NULL, NULL, connection_info);
        return NULL;
    }

    // Parse response to string and send it to client 
    char* message = response_to_string(response);
    if (message == NULL) {
        close_connection(fd, request, response, NULL, connection_info);
        return NULL;
    }

    size_t message_length = strlen(message);
    printf("Sending: %s\n\n", message);
    send(fd, message, message_length, 0);
    
    // Closing client socket connection
    close_connection(fd, request, response, message, connection_info);
    return NULL;
}

void close_connection(int fd, Request* request, Response* response, char* message, ConnectionInfo* connection_info) {
    if (request != NULL)
        request_free(request);
    
    if (response != NULL)
        response_free(response);

    if (message != NULL) 
        free(message);

    // Decrement connection count and free the connection info struct
    if (connection_info != NULL) {
        int* connections = connection_info->connections;
        pthread_mutex_t* connection_mutex = connection_info->connection_mutex;
    
        pthread_mutex_lock(connection_mutex);
        (*connections)--;
        pthread_mutex_unlock(connection_mutex);
        
        if (connection_info->address != NULL)
            free(connection_info->address);
        free(connection_info);
    }
    close(fd);
}