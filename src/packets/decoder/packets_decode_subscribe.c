/**
 * OpxFlow
 * Copyright (c) 2020 OperaMetrix SAS
 * Author: Nicolas Gonzalez <ngonzalez@operametrix.fr>
 *
 * This file is part of OpxFlow.
 *
 * OpxFlow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpxFlow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpxFlow.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include "packets/packets.h"
#include "packets/encoder/packets_encoder.h"
#include "log/log.h"
#include "backend/backend.h"

int packets_decode_subscribe(int fd, uint8_t *packet, size_t length)
{
    uint8_t i = 0;
    uint16_t packet_index;

    uint8_t control_packet_type = packet[0] && 0xF0;
    uint8_t control_packet_flags = packet[0] && 0x0F;
    uint8_t control_packet_remaining_length = packet[1];
    uint16_t message_identifier = 0;
    uint8_t topic_length = 0;
    uint16_t payload_length = 0;
    char topic[100];
    uint8_t requested_qos = 0;

    /* Discard fixed header */
    packet_index = 2;

    message_identifier = (packet[packet_index] << 8) + packet[packet_index + 1];

    /* Pass message identifier field */
    packet_index += 2;

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

    requested_qos = packet[packet_index];

    /* Pass requested qos field */
    packet_index += 1;

    if( packet_index != length )
    {
        LOG_ERROR("Extra bytes in the publish packet");
        return 0;
    }

    LOG_INFO("SUBSCRIBE %s (QoS %d)", topic, requested_qos);

    backend_add_subscriber(fd);

    LOG_DEBUG("<- SUBACK");
    packets_encode_suback(fd, message_identifier, requested_qos);

    return 1;
}