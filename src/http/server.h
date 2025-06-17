#pragma once

#include <fstream>
#include <filesystem>

#include "HTTPRequest.h"
#include "../connections/connection.h"

void handle_request(Connection conn, std::array<char, IN_BUF_SIZE>& recv_buf);
void respond_HTTPcode(Connection conn, int HTTPcode);
void respond_redirect(Connection conn, std::string& location);