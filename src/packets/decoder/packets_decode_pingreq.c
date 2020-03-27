#include <stdbool.h>
#include "packets/packets.h"
#include "packets/encoder/packets_encoder.h"
#include "log/log.h"

int packets_decode_pingreq(int fd, uint8_t *packet, size_t length)
{
    uint8_t i = 0;
    uint16_t packet_index;

    uint8_t control_packet_type = packet[0] && 0xF0;
    uint8_t control_packet_flags = packet[0] && 0x0F;
    uint8_t control_packet_remaining_length = packet[1];

    packet_index = 2;

    if( packet_index != length )
    {
        LOG_ERROR("Extra bytes in the publish packet");
        return 0;
    }

    LOG_DEBUG("<- PINGRESP");
    packets_encode_pingresp(fd);

    return 1;
}