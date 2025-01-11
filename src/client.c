#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "../include/client.h"

void* handle_client(void* arg) {
    client_t* client = (client_t*)arg;

    return NULL;
}

client_t* create_client(int clientfd, struct sockaddr_in client_addr) {
    client_t* client = malloc(sizeof(client_t));

    client->sockfd = clientfd;
    client->addr = client_addr;
    strcpy(client->username, "Guest"); 

    if (pthread_create(&client->thread_id, NULL, handle_client, (void*)client)
            != 0) {
        printf("Failed to create client thread: %s\n", strerror(errno));
        return NULL;
    }

    return client;
}

