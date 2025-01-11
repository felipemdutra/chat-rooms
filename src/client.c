#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../include/client.h"

client_t* create_client(int clientfd, struct sockaddr_in client_addr) {
    client_t* client = malloc(sizeof(client_t));

    client->sockfd = clientfd;
    client->addr = client_addr;
    strcpy(client->username, "Guest"); 



    return client;
}

