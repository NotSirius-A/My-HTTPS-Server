#include "connection.h"
#include "../ssl/ssl.h"
#include "../utils.h"

Connection::Connection(int conn_fd) {
    this->_conn_fd = conn_fd;
    this->_is_ssl = false;
}

Connection::Connection(int conn_fd, SSL_CTX* ssl_ctx) {
    this->_conn_fd = conn_fd;

    this->_ssl = init_ssl(conn_fd, ssl_ctx); 

    if (SSL_check_private_key(this->_ssl) == 1) {
        this->_is_ssl = true;
    } else {
        this->_is_ssl = false;
    }
}

int Connection::read(std::array<char, IN_BUF_SIZE>& recv_buf, int num) {
    int bytes_received;
    if (this->_is_ssl) {
        bytes_received = SSL_read(this->_ssl, recv_buf.data(), num);
    }  else {
        bytes_received = recv(this->_conn_fd, recv_buf.data(), num, 0);
    }

    return bytes_received;
}


int Connection::sendall(std::array<char, OUT_BUF_SIZE>& out_buf, int num) {
    
    int status;
    if (this->_is_ssl) {
        status = SSL_sendall(this->_ssl, out_buf, num);
    }  else {
        status = ::sendall(this->_conn_fd, out_buf, num);
    }

    return status;
}
