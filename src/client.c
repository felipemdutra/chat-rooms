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
#include "../include/room.h"
#include "../include/command.h"


client_t* clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * This function DOES NOT send the message to the person that sent it.
 *
 * @param sender The client who sent the message. They will NOT receive the message.
 * @param receiver The client who will receive the message.
 * 
 * @return 0 on success, -1 on failure.
 */
int send_message(client_t* sender, client_t* receiver, const char* message) {
    

    if (sender != receiver) {
        if (send(receiver->sockfd, message, strlen(message), 0) < 0) {
            send_system_message(sender, "Failed to send message");
            return -1;
        }
    }

    return 0;
}

int send_to_room(client_t* sender, const char* message) {
    if (!sender->curr_room) {
        send_system_message(sender, "You are not in a room.\nType \\join <room_name> to join a room\nType \\list to see available rooms\n");
        return -1;
    }

    room_t* room = sender->curr_room;
    char new_msg[1024];

    snprintf(new_msg, sizeof(new_msg), "%s > %s", sender->username, message);

    for (int i = 0; i < room->curr_num_clients; i++) {
        send_message(sender, room->clients[i], new_msg);
    }
    
    return 0;
}

int send_system_message(client_t* client, char* message) {
    if (message[0] == '\\') {
        return 0;
    }

    if (send_message(NULL, client, message) < 0) {
        return -1;
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
        handle_commands(client, buf);
        if (send_to_room(client, buf) < 0) {
            continue;
        }
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

/**
 * @param client The client that asked for the change of username.
 * @param new_name The name the client wants.
 *
 * @return 0 on success, -1 on failure.
 */
int change_name(client_t* client, const char* new_name) {
    if (strlen(new_name) > NAME_MAX_CHAR) {
        send_system_message(client, "Username is too big. Max characters: 20\n");
        return -1;
    }

    strcpy(client->username, new_name);
    return 0;
}

