#pragma once
#include "../main.h"

int init_server_socket(const int PORT);
int accept_connection(int server_socket_fd, sockaddr_in &client_addr);