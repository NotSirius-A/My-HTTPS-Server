#pragma once

#include "../main.h"

class Connection {
    public:
        const int get_fd() const {return this->_conn_fd;};
        const bool is_conn_ssl() const {return this->_is_ssl;};
        // const SSL* get_ssl() const {return this->_ssl;};
        void close() {::close(this->_conn_fd);};
        int read(std::array<char, IN_BUF_SIZE>& recv_buf, int num);
        int sendall(std::array<char, OUT_BUF_SIZE>& out_buf, int num);

        Connection(){};
        Connection(int conn_fd);
        Connection(int conn_fd, SSL_CTX* ssl_ctx);
    private:
        int _conn_fd;
        bool _is_ssl;
        SSL* _ssl;
};