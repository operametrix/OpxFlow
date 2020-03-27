#include <stdint.h>
#include <unistd.h>
#include "protocol/mqtt_protocol.h"

void packets_encode_suback(int fd, uint16_t message_identifier, uint8_t granted_qos)
{
    uint8_t control_packet_type = CMD_SUBACK;
    uint8_t remaining_length = 3;

    uint8_t packet[5] = {
        control_packet_type,
        remaining_length,
        message_identifier >> 8,
        message_identifier & 0xFF,
        granted_qos
    };

    write(fd, packet, 5);
}