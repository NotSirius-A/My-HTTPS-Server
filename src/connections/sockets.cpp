#include "sockets.h"



/**
 * @brief Initializes and returns listener socket file descriptor.
 * @return Socket file descriptor or -1 if error occurred.
 */
int init_server_socket(const int PORT) {

    // Create socket, get file descriptor
    int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket_fd < 0) {
        std::cout << "socket fd error" << std::endl;
        return -1;
    }

    // Set option to reuse address
    const int EN = 1;
    int setopt_status = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &EN, sizeof(server_socket_fd));

    // Bind socket to localhost at specified port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    std::memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    int bind_status = bind(server_socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)); 
    
    if (bind_status < 0) {
        std::cout << "bind error" << std::endl;
        perror("bind");
        return -1;
    }

    // Listen to incoming connections
    int listen_status = listen(server_socket_fd, QUEUE_SIZE);

    if (listen_status < 0) {
        std::cout << "listen error" << std::endl;
        return -1;
    }


    
    return server_socket_fd;
}

int accept_connection(int server_socket_fd, sockaddr_in &client_addr) {
    
    socklen_t length = sizeof(client_addr);
    int conn = accept(server_socket_fd, (sockaddr*)&client_addr, &length);

    if (conn < 0) {
        perror("connect");
        return -1;
    }

    std::cout << "accepted" << std::endl;


    return conn;
}