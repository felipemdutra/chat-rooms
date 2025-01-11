#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <pthread.h>

typedef struct {
    int sockfd;
    struct sockaddr_in;
    char username[20];
    pthread_t thread_id;
} client_t;



#endif

