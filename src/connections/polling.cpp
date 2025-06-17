#include "polling.h"


pollfd init_pollfd(int fd) {
    pollfd poll_obj;
    poll_obj.fd = fd;
    poll_obj.events = POLLIN;
    poll_obj.revents = 0;

    return poll_obj;
}