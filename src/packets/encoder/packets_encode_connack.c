#include <stdint.h>
#include <unistd.h>
#include "protocol/mqtt_protocol.h"

void packets_encode_connack(int fd)
{
    uint8_t control_packet_type = CMD_CONNACK;
    uint8_t remaining_length = 2;
    uint8_t connect_acknowledge_flags = 0;
    uint8_t connect_reason_code = 0;

    uint8_t packet[4] = {
        control_packet_type,
        remaining_length,
        connect_acknowledge_flags,
        connect_reason_code
    };

    write(fd, packet, 4);
}