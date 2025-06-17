#pragma once

#include "main.h"


int sendall(int socket, std::array<char, OUT_BUF_SIZE> &out_buf, size_t len);
int SSL_sendall(SSL* ssl_conn, std::array<char, OUT_BUF_SIZE> &out_buf, size_t len);
