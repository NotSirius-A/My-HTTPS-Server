#pragma once

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <cstring>
#include <poll.h>
#include <vector>
#include <array>
#include <cctype>
#include <algorithm>


const uint16_t PORT_HTTP = 8001;
const uint16_t PORT_HTTPS = 8000;
const std::string SERVER_HOST = "127.0.0.1";

const int QUEUE_SIZE = 10;
const size_t RESERVE_START_CONNECTIONS = 20;

const size_t IN_BUF_SIZE = 2048;
const size_t OUT_BUF_SIZE = 1024;


int sendall(int socket, std::array<char, OUT_BUF_SIZE> &out_buf, size_t len);