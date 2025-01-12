#ifndef ROOM_H
#define ROOM_H

#include "./client.h"

typedef struct {
    client_t** clients;
    int curr_num_clients;
    int max_clients;
    char* room_name;
} room_t;

room_t* create_room(int max_clients, const char* room_name);

#endif

