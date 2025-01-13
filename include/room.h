#ifndef ROOM_H
#define ROOM_H

#define MAX_ROOMS 15
#define ROOM_NAME_MAX_CHAR 20

#include "client.h"

typedef struct room_t {
    client_t* leader;
    client_t** clients;
    int curr_num_clients;
    int max_clients;
    char room_name[ROOM_NAME_MAX_CHAR];
} room_t;


room_t* create_room(const char *room_name, int max_clients, client_t* creator);
int delete_room(const char* room_name, client_t* owner);
room_t* find_room(const char* room_name);
int join_room(client_t* client, char* room_name);
room_t** get_rooms();
int exit_room(client_t* client, room_t* room);

#endif

