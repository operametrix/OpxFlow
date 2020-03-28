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

#include <stdint.h>
#include <unistd.h>
#include "protocol/mqtt_protocol.h"

void packets_encode_connack(int fd)
{
    uint8_t control_packet_type = CMD_CONNACK;
    uint8_t remaining_length = 3;
    uint8_t connect_acknowledge_flags = 0;
    uint8_t connect_reason_code = CONNACK_SUCCESS;
    uint8_t topic_alias_maximum = 0;

    uint8_t packet[5] = {
        control_packet_type,
        remaining_length,
        connect_acknowledge_flags,
        connect_reason_code,
        topic_alias_maximum
    };

    write(fd, packet, 5);
}