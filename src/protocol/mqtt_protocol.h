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

#ifndef __MQTT_PROTOCOL_H__
#define __MQTT_PROTOCOL_H__

#define PROTOCOL_NAME_v31 "MQIsdp"
#define PROTOCOL_VERSION_v31 3

#define PROTOCOL_NAME "MQTT"

#define PROTOCOL_VERSION_v311 4
#define PROTOCOL_VERSION_v5 5

/* Message types */
#define CMD_CONNECT 0x10
#define CMD_CONNACK 0x20
#define CMD_PUBLISH 0x30
#define CMD_PUBACK 0x40
#define CMD_PUBREC 0x50
#define CMD_PUBREL 0x60
#define CMD_PUBCOMP 0x70
#define CMD_SUBSCRIBE 0x80
#define CMD_SUBACK 0x90
#define CMD_UNSUBSCRIBE 0xA0
#define CMD_UNSUBACK 0xB0
#define CMD_PINGREQ 0xC0
#define CMD_PINGRESP 0xD0
#define CMD_DISCONNECT 0xE0
#define CMD_AUTH 0xF0

/* Properties types */
#define PROP_PAYLOAD_FORMAT_INDICATOR 0x01
#define PROP_MESSAGE_EXPIRY_INTERVAL 0x02
#define PROP_CONTENT_TYPE 0x03
#define PROP_RESPONSE_TOPIC 0x08
#define PROP_CORRELATION_DATA 0x09
#define PROP_SUBSCRIPTION_IDENTIFIER 0x0B
#define PROP_SESSION_EXPIRY_INTERVAL 0x11
#define PROP_ASSIGNED_CLIENT_IDENTIFIER 0x12
#define PROP_SERVER_KEEP_ALIVE 0x13
#define PROP_AUTHENTICATION_METHOD 0x15
#define PROP_AUTHENTICATION_DATA 0x16
#define PROP_REQUEST_PROBLEM_INFORMATION 0x17
#define PROP_WILL_DELAY_INTERVAL 0x18
#define PROP_REQUEST_RESPONSE_INFORMATION 0x19
#define PROP_RESPONSE_INFORMATION 0x1A
#define PROP_SERVER_REFERENCE 0x1C
#define PROP_REASON_STRING 0x1F
#define PROP_RECEIVE_MAXIMUM 0x21
#define PROP_TOPIC_ALIAS_MAXIMUM 0x22
#define PROP_TOPIC_ALIAS 0x23
#define PROP_MAXIMUM_QOS 0x24
#define PROP_RETAIN_AVAILABLE 0x25
#define PROP_USER_PROPERTY 0x26
#define PROP_MAXIMUM_PACKET_SIZE 0x27
#define PROP_WILDCARD_SUBSCRIPTION_AVAILABLE 0x28
#define PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE 0x29
#define PROP_SHARED_SUBSCRIPTION_AVAILABLE 0x2A

/* Connect reason code values */
#define CONNACK_SUCCESS 0x00
#define CONNACK_UNSPECIFIED_ERROR 0x80
#define CONNACK_MALFORMED_PACKET 0x81
#define CONNACK_PROTOCOL_ERROR 0x82
#define CONNACK_IMPLEMENTATION_SPECIFIC_ERROR 0x83
#define CONNACK_UNSUPPORTED_PROTOCOL_VERSION 0x84
#define CONNACK_CLIENT_IDENTIFIER_NOT_VALID 0x85
#define CONNACK_BAD_USERNAME_OF_PASSWORD 0x86
#define CONNACK_NOT_AUTHORIZED 0x87
#define CONNACK_SERVER_UNAVAILABLE 0x88
#define CONNACK_SERVER_BUSY 0x89
#define CONNACK_BANNED 0x8A
#define CONNACK_BAD_AUTHENTICATION_METHOD 0x8C
#define CONNACK_TOPIC_NAME_INVALID 0x90
#define CONNACK_PACKET_TOO_LARGE 0x95
#define CONNACK_QUOTA_EXCEEDED 0x97
#define CONNACK_PAYLOAD_FORMAT_INVALID 0x99
#define CONNACK_RETAIN_NOT_SUPPORTED 0x9A
#define CONNACK_QOS_NOT_SUPPORTED 0x9B
#define CONNACK_USE_ANOTHER_SERVER 0x9C
#define CONNACK_SERVER_MOVED 0x9D
#define CONNACK_CONNECTION_RATE_EXCEEDED 0x9F

#endif