#ifndef COMMAND_H
#define COMMAND_H

#include "../include/client.h"

int send_system_message(client_t* client, char* message);
void handle_commands(client_t* client, char* buf);

#endif

