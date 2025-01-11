#include <unistd.h>

#include "../include/server.h"
#include "../include/socket.h"

int start_server() {
    int server_sock = create_socket();
    int client_sock;

    if (server_sock < 0) {
        return -1;
    }

    if (enable_socket_reuseaddr(server_sock) < 0) {
        close(server_sock);
        return -1;
    }

    if (bind_socket(server_sock) < 0) {
        close(server_sock);
        return -1;
    }

    while (1) {
        if (listen_for_connections(server_sock) < 0) {
            close(server_sock);
            return -1;
        }

        client_sock = accept_client(server_sock);
        if (client_sock < 0) {
            close(server_sock);
            return -1;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
