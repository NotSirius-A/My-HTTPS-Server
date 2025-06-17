#pragma once

#include <vector>
#include <poll.h>

void init_polling(int server_socket_fd, std::vector<pollfd> &poll_sockets);
pollfd init_pollfd(int fd);