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