#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <pthread.h>

typedef struct {
    int sockfd;
    struct sockaddr_in addr;
    char username[20];
    pthread_t thread_id;
} client_t;

client_t* create_client(int clientfd, struct sockaddr_in client_addr);

#endif

