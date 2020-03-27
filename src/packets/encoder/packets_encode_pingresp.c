#include <stdint.h>
#include <unistd.h>
#include "protocol/mqtt_protocol.h"

void packets_encode_pingresp(int fd)
{
    uint8_t control_packet_type = CMD_PINGRESP;
    uint8_t remaining_length = 0;

    uint8_t packet[4] = {
        control_packet_type,
        remaining_length,
    };

    write(fd, packet, 2);
}