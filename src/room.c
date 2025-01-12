#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../include/room.h"

/**
 * @param room_name The name of the room.
 * @param max_clients The max number of clients the room can hold.
 * @param creator A pointer to the client who created the room.
 *
 * @return A pointer to a room on success, NULL on failure.
 */
room_t* create_room(char *room_name, int max_clients, client_t* creator) {
    room_t* room = malloc(sizeof(room_t));
    if (!room) {
        printf("Failed to create room: %s\n", strerror(errno));
        return NULL;
    }

    room->room_name = room_name;
    room->max_clients = max_clients;
    room->leader = creator;
    room->curr_num_clients = 0;

    return room;
}

