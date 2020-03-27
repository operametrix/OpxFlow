#ifndef __SERVER_H__
#define __SERVER_H__

#include <netinet/in.h>

void server_run(int listener_fd);
int server_create_listener(struct in_addr address, uint16_t port);

#endif