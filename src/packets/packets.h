#ifndef __PACKETS_H__
#define __PACKETS_H__

#include <stdint.h>
#include <unistd.h>

int packets_read(int connection_fd);
int packets_write(int connection_fd, int8_t *buffer, size_t length);

#endif