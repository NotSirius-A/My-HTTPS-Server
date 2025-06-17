#include "main.h"

#include "./connections/sockets.h"
#include "./connections/polling.h"
#include "./connections/connection.h"
#include "./http/server.h"
#include "./ssl/ssl.h"


#include <errno.h>
#include <unordered_map>



int main() {

    int http_server_socket_fd = init_server_socket(PORT_HTTP);
    int https_server_socket_fd = init_server_socket(PORT_HTTPS);

    std::vector<pollfd> poll_sockets;
    poll_sockets.reserve(RESERVE_START_CONNECTIONS);

    std::unordered_map<int, Connection> connections;

    // HTTP socket at index 0
    pollfd http_socket_poll = init_pollfd(http_server_socket_fd);
    poll_sockets.insert(poll_sockets.cbegin(), http_socket_poll);


    // HTTPS socket at index 1
    pollfd https_socket_poll = init_pollfd(https_server_socket_fd);
    poll_sockets.insert(poll_sockets.cbegin()+1, https_socket_poll);

    std::array<char, IN_BUF_SIZE> recv_buf = {};

    SSL_CTX* server_ssl_ctx = init_ssl_ctx();
    

    std::cout << "listening" << std::endl;
    while (true) {

        int num_events = poll(&poll_sockets[0], poll_sockets.size(), 0);

        if (num_events > 0) {

            for (int i = 0; i < poll_sockets.size(); i++) {
                bool event_ready = poll_sockets[i].revents > 0;
                if (!event_ready) {
                    continue;
                }


                if (i == 0) {
                    pollfd &server_socket_poll = poll_sockets[0];

                    if (server_socket_poll.revents & POLLIN) {
                        sockaddr_in client_addr;

                        int conn = accept_connection(http_server_socket_fd, client_addr);
                        if (conn > 0) {                           
                            
                            pollfd client_poll = init_pollfd(conn);
                            poll_sockets.push_back(client_poll);

                            Connection conn(client_poll.fd);
                            connections[client_poll.fd] = conn;

                        } else {
                            std::cout << "error accepting http" << std::endl;
                        }
                    }

                } else if (i == 1) {
                    pollfd &server_socket_poll = poll_sockets[1];

                    if (server_socket_poll.revents & POLLIN) {
                        sockaddr_in client_addr;

                        int conn = accept_connection(https_server_socket_fd, client_addr);
                        if (conn > 0) {                           
                            
                            pollfd client_poll = init_pollfd(conn);
                            poll_sockets.push_back(client_poll);

                            Connection conn(client_poll.fd, server_ssl_ctx);
                            connections[client_poll.fd] = conn;

                        } else {
                            std::cout << "error accepting https" << std::endl;
                        }
                    }
                } else {
                    pollfd &socket_poll = poll_sockets[i];
                    Connection conn = connections[socket_poll.fd];

                    if (socket_poll.revents & (POLLIN | POLLHUP)) {
                        int bytes_received = conn.read(recv_buf, recv_buf.size());

                        if (bytes_received > 0) {
                            // if (fork() == 0) {
                                handle_request(conn, recv_buf);
                                recv_buf.fill(0);
                                // exit(0);
                            // }
                            
                        } else {
                            conn.close();
                            poll_sockets.erase(poll_sockets.begin() + i);
                            connections.erase(socket_poll.fd);
                            std::cout << "dc" << std::endl;
                        }
                    }
                }
            }


        }
    }

    std::cout << "end" << std::endl;
    close(http_server_socket_fd);
    close(https_server_socket_fd);
    return 0;
}