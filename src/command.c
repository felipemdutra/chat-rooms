#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

extern int room_count;

#include "../include/client.h"
#include "../include/command.h"
#include "../include/room.h"

void handle_commands(client_t* client, char* buf) {
    if (buf[0] != '\\') {
        return;
    }

    char command[256];
    char args[256] = "";
    if (sscanf(buf, "\\%255s %255[^\n]", command, args) < 1) {
        send_system_message(client, "Invalid command format.\n");
    }

    if (strcmp(command, "name") == 0) {
        change_name(client, args);
    }

    if (strcmp(command, "create") == 0) {
        if (create_room(args, 10, client) == NULL) {
            return;
        }
    }

    if (strcmp(command, "join") == 0) {
        join_room(client, args);
    }

    if (strcmp(command, "list") == 0) {
       room_t** rooms = get_rooms();
        if (!rooms) {
            send_system_message(client, "Failed to retrieve rooms list");
            return;
        }

        if (room_count == 0) {
            send_system_message(client, "No rooms available");
            return;
        } 
        char room_list[1024] = "Available rooms:\n";
        
        for (int i = 0; i < room_count; i++) {
            strncat(room_list, rooms[i]->room_name, sizeof(room_list) - strlen(room_list) - 1);
            strncat(room_list, "\n", sizeof(room_list) - strlen(room_list) - 1);
        }

        send_system_message(client, room_list);
        free(rooms);
    }
}

