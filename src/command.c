#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#include "../include/client.h"
#include "../include/command.h"
#include "../include/room.h"

void handle_commands(client_t* client, char* buf) {
    if (buf[0] != '\\') {
        return;
    }

    char command[256];
    char args[256] = {0};
    if (sscanf(buf, "\\%s %[^\n]", command, args) < 1) {
        send_system_message(client, "Invalid command format.\n");
    }

    if (strcmp(command, "name") == 0) {
        change_name(client, args);
    }

    if (strcmp(command, "create") == 0) {
        room_t* room = create_room(args, 10, client);
    }
}

