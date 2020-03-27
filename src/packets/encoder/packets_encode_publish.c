#include <stdint.h>
#include <unistd.h>
#include "protocol/mqtt_protocol.h"

void packets_encode_publish(int fd, uint8_t *topic, size_t topic_len, uint8_t *payload, size_t payload_len)
{
    uint8_t control_packet_type = CMD_PUBLISH;
    uint8_t remaining_length = 3;

    size_t packet_len = payload_len + topic_len + 4;
    uint8_t packet[packet_len];

    packet[0] = control_packet_type;
    packet[1] = payload_len + topic_len + 2;
    packet[2] = topic_len >> 8;
    packet[3] = (uint8_t) (topic_len & 0xFF);

    int i;
    for(i=0; i<topic_len; i++)
    {
        packet[4 + i] = topic[i];
    }

    for(i=0; i<payload_len; i++)
    {
        packet[4 + topic_len + i] = payload[i];
    }

    write(fd, packet, packet_len);
}