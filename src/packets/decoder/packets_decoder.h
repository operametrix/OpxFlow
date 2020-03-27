#ifndef __PACKETS_DECODER_H__
#define __PACKETS_DECODER_H__

#include <stdint.h>
#include <unistd.h>

int packets_decode_connect(int fd, uint8_t *packet, size_t length);
int packets_decode_publish(int fd, uint8_t *packet, size_t length);
int packets_decode_subscribe(int fd, uint8_t *packet, size_t length);
int packets_decode_pingreq(int fd, uint8_t *packet, size_t length);

#endif