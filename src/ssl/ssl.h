#include <openssl/ssl.h>

SSL_CTX* init_ssl_ctx();
SSL* init_ssl(int socket_fd, SSL_CTX* ctx);