#ifndef SOCKET_H
#define SOCKET_H

#define PORT 8080

int create_socket();
int bind_socket(int sockfd);
int listen_for_connections(int sockfd);
int enable_socket_reuseaddr(int sockfd);
int accept_client(int sockfd);

#endif

