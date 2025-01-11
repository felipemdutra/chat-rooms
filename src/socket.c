#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <errno.h>

#include "../include/socket.h"

#define MAX_CLIENT_CONNECTIONS 10 

/**
 * Creates a TCP socket.
 *
 * @return Socket file descriptor on success, -1 on failure.
 */
int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Failed to create socket: %s\n", strerror(errno));
        return -1;
    }

    return sockfd;
}

/**
 * Binds the given socket to the port defined in include/socket.h.
 *
 * @param sockfd The socket file descriptor.
 * @return 0 on success, -1 on failure.
 */
int bind_socket(int sockfd) {
    struct sockaddr_in sock_addr;

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(PORT);
    sock_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
        printf("Faile to bind socket: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

/**
 * Listens for incoming client connections.
 *
 * @param sockfd The socket file descriptor.
 * @return 0 on success, -1 on failure.
 */
int listen_for_connections(int sockfd) {
    if (listen(sockfd, MAX_CLIENT_CONNECTIONS) < 0) {
        printf("Failed to listen to connections: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

/**
 * Accepts a client connection.
 *
 * @param serverfd The server socket file descriptor.
 * @return Client socket file descriptor on success, -1 on failure.
 */
int accept_client(int serverfd) {
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int client_sock = accept(serverfd, (struct sockaddr*)&client_addr, &client_addrlen);
    if (client_sock < 0) {
        printf("Failed to accept client connection: %s\n", strerror(errno));
        return -1;
    }

    return client_sock;
}

/**
 * Enables the SO_REUSEADDR option on the socket.
 *
 * @param sockfd The socket file descriptor.
 * @return 0 on success, -1 on failure.
 */
int enable_socket_reuseaddr(int sockfd) {
    const int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("Failed to enable socket reuseaddr option: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

