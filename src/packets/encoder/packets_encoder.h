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

#ifndef __PACKETS_ENCODER_H__
#define __PACKETS_ENCODER_H__

#include <stdint.h>
#include <unistd.h>

void packets_encode_connack(int fd);
void packets_encode_suback(int fd, uint16_t message_identifier, uint8_t granted_qos);
void packets_encode_pingresp(int fd);
void packets_encode_publish(int fd, uint8_t *topic, size_t topic_len, uint8_t *payload, size_t payload_len);

#endif