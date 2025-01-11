#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>

#include "../include/client.h"

/**
 * @param arg Pointer to client.
 */
void* handle_client(void* arg) {
    client_t* client = (client_t*)arg;

    return NULL;
}

/**
 * @param clientfd Client socket descriptor.
 * @param client_addr Client address.
 *
 * @return Pointer to the created client structure, or NULL on failure.
 */
client_t* create_client(int clientfd, struct sockaddr_in client_addr) {
    client_t* client = malloc(sizeof(client_t));
    if (!client) {
        printf("Failed to allocate memory for client\n");
        return NULL;
    }

    client->sockfd = clientfd;
    client->addr = client_addr;
    strcpy(client->username, "Guest"); 

    if (pthread_create(&client->thread_id, NULL, handle_client, (void*)client)
            != 0) {
        printf("Failed to create client thread: %s\n", strerror(errno));
        free(client);
        return NULL;
    }

    return client;
}

