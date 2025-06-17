#include "main.h"

#include "utils.h"

int sendall(int socket, std::array<char, OUT_BUF_SIZE> &out_buf, size_t len) {
    int bytes_sent = 0;
    int n = 0;

    while (bytes_sent < len) {
        void* p_send_buf = out_buf.data()+bytes_sent;
        n = send(socket, p_send_buf, len-bytes_sent, 0);
        bytes_sent += n;
    }

    return (n == -1) ? -1 : 0;
}


int SSL_sendall(SSL* ssl_conn, std::array<char, OUT_BUF_SIZE> &out_buf, size_t len) {
    int bytes_sent = 0;
    int n = 0;

    while (bytes_sent < len) {
        void* p_send_buf = out_buf.data()+bytes_sent;
        n = SSL_write(ssl_conn, p_send_buf, len-bytes_sent);
        bytes_sent += n;
    }

    return (n == -1) ? -1 : 0;
}



