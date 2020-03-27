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

#include "decoder/packets_decoder.h"
#include "protocol/mqtt_protocol.h"
#include "log/log.h"

static int packets_inbound(int fd, uint8_t *packet, size_t length)
{
    uint8_t control_packet_type = packet[0] & 0xF0;
    int decode_status = 1;

    switch(control_packet_type)
    {
        case CMD_CONNECT:
            LOG_DEBUG("-> CONNECT");
            decode_status = packets_decode_connect(fd, packet, length);
            break;

        case CMD_CONNACK:
            break;

        case CMD_PUBLISH:
            LOG_DEBUG("-> PUBLISH");
            decode_status = packets_decode_publish(fd, packet, length);
            break;

        case CMD_PUBACK:
            break;

        case CMD_PUBREC:
            break;

        case CMD_PUBREL:
            break;

        case CMD_PUBCOMP:
            break;

        case CMD_SUBSCRIBE:
            LOG_DEBUG("-> SUBSCRIBE");
            decode_status = packets_decode_subscribe(fd, packet, length);
            break;

        case CMD_SUBACK:
            break;

        case CMD_UNSUBSCRIBE:
            break;

        case CMD_UNSUBACK:
            break;

        case CMD_PINGREQ:
            LOG_DEBUG("-> PINGREQ");
            decode_status = packets_decode_pingreq(fd, packet, length);
            break;

        case CMD_PINGRESP:
            break;

        case CMD_DISCONNECT:
            decode_status = 0;
            LOG_DEBUG("-> DISCONNECT");
            break;

        case CMD_AUTH:
            break;
    }

    return decode_status;
}

int packets_read(int connection_fd)
{
    uint8_t buff[100];
    ssize_t nb_bytes;

    uint8_t control_packet_type;
    uint8_t control_packet_flags;
    uint8_t remaining_length;

    nb_bytes = read(connection_fd, buff, 2);
    if(nb_bytes > 0)
    {
        control_packet_type = buff[0] & 0xF0;
        control_packet_flags = buff[0] & 0x0F;
        remaining_length = buff[1];
    }
    else
    {
        return 0;
    }

    if( remaining_length > 0 )
    {
        nb_bytes = read(connection_fd, buff+2, remaining_length);
        nb_bytes += 2;
    }

    if(nb_bytes > 0)
    {
        if( packets_inbound(connection_fd, buff, (size_t) nb_bytes) == 0)
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return 1;
}

int packets_write(int connection_fd, int8_t *buffer, size_t length)
{
    ssize_t nb_bytes = 0;
    nb_bytes = write(connection_fd, buffer, length);

    if( nb_bytes > 0 )
    {

    }
    else if( nb_bytes == 0 )
    {

    }
    else
    {

    }
    
    return 1;
}