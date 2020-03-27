#ifndef __PACKETS_ENCODER_H__
#define __PACKETS_ENCODER_H__

#include <stdint.h>
#include <unistd.h>

void packets_encode_connack(int fd);
void packets_encode_suback(int fd, uint16_t message_identifier, uint8_t granted_qos);
void packets_encode_pingresp(int fd);
void packets_encode_publish(int fd, uint8_t *topic, size_t topic_len, uint8_t *payload, size_t payload_len);

#endif