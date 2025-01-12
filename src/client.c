#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/client.h"

#define MAX_CLIENTS 100

client_t* clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int send_global_message(client_t* sender, const char *message) {
    char buf[1100];

    snprintf(buf, sizeof(buf), "%s > %s", sender->username, message);
    printf("%s\n", sender->username);

    for (int i = 0; i < num_clients; i++) {
        if (clients[i] == sender) {
            continue;
        }

        if (send(clients[i]->sockfd, buf, strlen(buf), 0) < 0) {
            printf("Failed to send global message: %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

ssize_t read_message(client_t* client, char* buf, size_t buf_size) {
    ssize_t bytes_read = recv(client->sockfd, buf, buf_size, 0);
    if (bytes_read <= 0) {
        printf("%s disconnected\n", client->username);
        return -1;
    }

    return bytes_read;
}

/**
 * @param arg Pointer to client.
 */
void* handle_client(void* arg) {
    client_t* client = (client_t*)arg;
    char buf[1024];
    
    while (1) {
        ssize_t bytes_read = read_message(client, buf, sizeof(buf));
        if (bytes_read < 0) {
            break;
        }
        buf[bytes_read] = '\0';
        send_global_message(client, buf);
    }

    close(client->sockfd);
    pthread_exit("Client");
    return NULL;
}

/**
 * @param clientfd Client socket descriptor.
 * @param client_addr Client address.
 *
 * @return Pointer to the created client structure, or NULL on failure.
 */
client_t* create_client(int clientfd) {
    client_t* client = malloc(sizeof(client_t));
    if (!client) {
        printf("Failed to allocate memory for client\n");
        return NULL;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    getpeername(clientfd, (struct sockaddr*)&client_addr, &client_addrlen);

    client->sockfd = clientfd;
    client->addr = client_addr;
    strcpy(client->username, "Guest"); 

    if (pthread_create(&client->thread_id, NULL, handle_client, (void*)client)
            != 0) {
        printf("Failed to create client thread: %s\n", strerror(errno));
        free(client);
        return NULL;
    }

    pthread_mutex_lock(&clients_mutex);

    if (num_clients >= MAX_CLIENTS) {
        printf("Maximum clients reached, cannot add more clients");
        return NULL;
    } else {
        clients[num_clients] = client;
        num_clients++;
    }

    pthread_mutex_unlock(&clients_mutex);

    return client;
}

