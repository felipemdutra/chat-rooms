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

void* handle_client(void* arg);
client_t* create_client(int clientfd);
ssize_t read_message(client_t* client, char* buf, size_t buf_size);
int send_global_message(client_t* sender, const char* message);

#endif

