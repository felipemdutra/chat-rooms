#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../include/room.h"

room_t* rooms[MAX_ROOMS];
pthread_mutex_t rooms_mutex = PTHREAD_MUTEX_INITIALIZER;
int room_count = 0;

/**
 * @param room_name The name of the room.
 * @param max_clients The max number of clients the room can hold.
 * @param creator A pointer to the client who created the room.
 *
 * @return A pointer to a room on success, NULL on failure.
 */
room_t* create_room(const char *room_name, int max_clients, client_t* creator) {
    if (room_count >= MAX_ROOMS) {
        send_system_message(creator, "Room limit reached. Cannot create more rooms\n");
        return NULL;
    }

    if (!room_name) {
        send_system_message(creator, "Invalid room name\n");
        return NULL;
    }

    room_t* room = malloc(sizeof(room_t));
    if (!room) {
        printf("Failed to create room: %s\n", strerror(errno));
        return NULL;
    }

    strcpy(room->room_name, room_name);
    room->max_clients = max_clients;
    room->leader = creator;
    room->curr_num_clients = 0;
    room->clients = NULL;

    room->clients = malloc(sizeof(max_clients));
    if (!room->clients) {
        free(room);
        printf("Failed to allocate memory for room clients: %s\n", strerror(errno));
        return NULL;
    }

    pthread_mutex_lock(&rooms_mutex);

    rooms[room_count] = room;
    room_count++;

    pthread_mutex_unlock(&rooms_mutex);

    send_system_message(creator, "Room created. Use \\list to view it\n");

    return room;
}

room_t* find_room(const char* room_name) {
    for (int i = 0; i < room_count; i++) {
        if (strcmp(rooms[i]->room_name, room_name) == 0) {
            return rooms[i];
        }
    }

    return NULL;
}

int delete_room(const char* room_name, client_t* client) {
    room_t* room = find_room(room_name);
    if (!room) {
        send_system_message(client, "Room doesn't exist\n");
        return -1;
    }

    if (strcmp(room->leader->username, client->username) != 0) {
        send_system_message(client, "You cannot delete the room because you do not own it\n");
        return -1;
    }

    free(room->clients);
    free(room);

    pthread_mutex_lock(&rooms_mutex);
    /* TODO: Remove freed room from rooms array */
    room_count--;
    pthread_mutex_unlock(&rooms_mutex);

    return 0;
}

int join_room(client_t* client, char* room_name) {
    room_t* room = find_room(room_name);
    if (!room) {
        send_system_message(client, "Room doesn't exist\n");
        return -1;
    }

    if (room->curr_num_clients >= room->max_clients) {
        send_system_message(client, "Room is full\n");
        return -1;
    }

    pthread_mutex_lock(&rooms_mutex);

    room->clients[room->curr_num_clients] = client;
    room->curr_num_clients++;

    pthread_mutex_unlock(&rooms_mutex);

    send_system_message(client, "You joined a room. Type \\exit to exit\n");
    return 0;
}
