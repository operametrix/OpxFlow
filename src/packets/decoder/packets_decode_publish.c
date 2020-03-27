#include <stdbool.h>
#include "packets/packets.h"
#include "packets/encoder/packets_encoder.h"
#include "log/log.h"
#include "backend/backend.h"

int packets_decode_publish(int fd, uint8_t *packet, size_t length)
{
    uint8_t i = 0;
    uint16_t packet_index;

    uint8_t control_packet_type = packet[0] && 0xF0;
    uint8_t control_packet_flags = packet[0] && 0x0F;
    uint8_t control_packet_remaining_length = packet[1];
    uint8_t topic_length = 0;
    uint16_t payload_length = 0;
    char topic[100];
    char payload[100];

    /* Discard fixed header */
    packet_index = 2;

    topic_length = (packet[packet_index] << 8) + packet[packet_index + 1];

    /* Pass topic length field */
    packet_index += 2;

    char c = 0;
    for(i=0; i < topic_length; i++)
    {
        c = packet[packet_index + i];
        topic[i] = c;
    }
    topic[i] = 0;

    /* Pass topic field */
    packet_index += topic_length;

    payload_length = control_packet_remaining_length - topic_length - 2;

    for(i=0; i < payload_length; i++)
    {
        c = packet[packet_index + i];
        payload[i] = c;
    }
    payload[i] = 0;

    /* Pass payload field */
    packet_index += payload_length;

    if( packet_index != length )
    {
        LOG_ERROR("Extra bytes in the publish packet");
        return 0;
    }

    /* EXPERIMENTAL */
    int nb_s = backend_get_subscribers_size();
    int sub;
    for(i=0; i<nb_s; i++)
    {
        sub = backend_get_subscriber(i);
        packets_encode_publish(sub, (uint8_t *)topic, topic_length, (uint8_t *)payload, payload_length);
    }
    /* --- */

    LOG_INFO("PUBLISH %s %s", topic, payload);

    return 1;
}