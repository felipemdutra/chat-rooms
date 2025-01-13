#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <pthread.h>

struct room_t;

#define MAX_CLIENTS 100
#define NAME_MAX_CHAR 20

typedef struct {
    int sockfd;
    struct sockaddr_in addr;
    char username[NAME_MAX_CHAR];
    pthread_t thread_id;
    struct room_t* curr_room;
} client_t;

void* handle_client(void* arg);
client_t* create_client(int clientfd);
ssize_t read_message(client_t* client, char* buf, size_t buf_size);
int send_system_message(client_t* client, char* message);
int send_global_message(client_t* sender, const char* message);
int send_to_room(client_t* sender, const char* message);
int send_message(client_t* sender, client_t* receiver, const char* message);

int change_name(client_t* client, const char* new_name);

#endif

