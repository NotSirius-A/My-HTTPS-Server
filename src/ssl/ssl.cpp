#include <filesystem>

#include "ssl.h"

#include <iostream>

SSL_CTX* init_ssl_ctx() {
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());


    namespace fs = std::filesystem;
    fs::path cert_path = fs::current_path();
    cert_path.append("ssl/cert.pem");

    fs::path key_path = fs::current_path();
    key_path.append("ssl/key.pem");

    std::cout << cert_path << " | " << key_path << std::endl;

    int status;
    int error;
    status = SSL_CTX_use_certificate_file(ctx, cert_path.c_str(), SSL_FILETYPE_PEM);
    
    if (status < 1) {
        std::cout << "cert load error" << std::endl;
    }

	status = SSL_CTX_use_PrivateKey_file(ctx, key_path.c_str(), SSL_FILETYPE_PEM);

    if (status < 1) {
        std::cout << "key load error" << std::endl;
    }

    return ctx;

}


SSL* init_ssl(int socket_fd, SSL_CTX* ctx) {
    
	SSL* ssl = SSL_new(ctx);
    
    int status;
    int error;
	status = SSL_set_fd(ssl, socket_fd);
    error = SSL_get_error(ssl, status);
    
    int accept = SSL_accept(ssl);

    return ssl;

}