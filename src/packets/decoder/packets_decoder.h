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

#ifndef __PACKETS_DECODER_H__
#define __PACKETS_DECODER_H__

#include <stdint.h>
#include <unistd.h>

int decode_variable_byte(uint8_t *buffer, size_t index, uint32_t *value);

int packets_decode_connect(int fd, uint8_t *packet, size_t length);
int packets_decode_publish(int fd, uint8_t *packet, size_t length);
int packets_decode_subscribe(int fd, uint8_t *packet, size_t length);
int packets_decode_pingreq(int fd, uint8_t *packet, size_t length);

#endif