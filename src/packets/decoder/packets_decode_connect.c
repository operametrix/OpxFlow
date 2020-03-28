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
#include "protocol/mqtt_protocol.h"
#include "packets/packets.h"
#include "packets/decoder/packets_decoder.h"
#include "packets/encoder/packets_encoder.h"
#include "log/log.h"

int packets_decode_connect(int fd, uint8_t *packet, size_t length)
{
    uint8_t i = 0;
    uint16_t packet_index;

    uint8_t control_packet_type = packet[0] && 0xF0;
    uint8_t control_packet_flags = packet[0] && 0x0F;
    uint8_t control_packet_remaining_length = packet[1];
    uint16_t protocol_name_length = 0;
    uint8_t protocol_version = 0;
    bool username_flag = false;
    bool password_flag = false;
    bool will_retain_flag = false;
    uint8_t will_qos = 0;
    bool will_flag = false;
    bool clean_start_flag = false;
    bool reserved_flag = false;
    uint16_t keep_alive = 0;
    uint32_t properties_field_length = 0;
    uint8_t properties_field_name = 0;
    uint32_t will_properties_field_length = 0;
    uint8_t will_properties_field_name = 0;
    uint16_t payload_field_length = 0;
    char client_id[23];
    char username[100];
    char password[100];

    /* Discard fixed header */
    packet_index = 2;

    protocol_name_length = (packet[packet_index] << 8) + packet[packet_index + 1];

    if( protocol_name_length != 4 )
    {
        LOG_ERROR("bad protocol name length");
        return 0;
    }

    /* Pass protocol name length field */
    packet_index += 2;

    /* [MQTT-3.1.2-1]
        The protocol name MUST be the UTF-8 String "MQTT".
        If the Server does not want to accept the CONNECT, and wishes to revealthat it is an MQTT Server 
        it MAY send a CONNACK packet with ReasonCode of 0x84 (Unsupported Protocol Version), 
        and then it MUST close the Network Connection
    */
    if( packet[packet_index] != 'M' || packet[packet_index + 1] != 'Q' || packet[packet_index + 2] != 'T' || packet[packet_index + 3] != 'T' )
    {
        LOG_ERROR("bad protocol name");
        return 0;
    }

    /* Pass protocol name field */
    packet_index += 4;

    protocol_version = packet[packet_index];

    /* Pass protocol version field */
    packet_index += 1;

    username_flag = (packet[9] & 0x80) ? 1 : 0;
    password_flag = (packet[9] & 0x40) ? 1 : 0;
    will_retain_flag = (packet[9] & 0x20) ? 1 : 0;
    will_qos = (packet[9] & 0x18) >> 3;
    will_flag = (packet[9] & 0x04) ? 1 : 0;
    clean_start_flag = (packet[9] & 0x02) ? 1 : 0;
    reserved_flag = (packet[9] & 0x01) ? 1 : 0;

    /* [MQTT-3.1.2-3]
        The Server MUST validate that the reserved flag in the CONNECT packet is set to 0
    */
    if( reserved_flag != 0 )
    {
        LOG_ERROR("[MQTT-3.1.2-3] reserved flag is set");
        return 0;
    }

    if( will_flag == 0 )
    {
        /* [MQTT-3.1.2-11]
            If the Will Flag is set to 0, then the Will QoS MUST be set to 0(0x00)
        */
        if( will_qos != 0 )
        {
            LOG_ERROR("[MQTT-3.1.2-3] bad will qos value");
            return 0;
        }

        /* [MQTT-3.1.2-13]
            If the Will Flag is set to 0, then Will Retain MUST be set to 0
        */
        if( will_retain_flag != 0 )
        {
            LOG_ERROR("[MQTT-3.1.2-13] retain flag must not be set");
            return 0;
        }
    }
    else
    {
        /* [MQTT-3.1.2-12]
            If the Will Flag is set to 1, the value of Will QoS can be 0 (0x00), 1 (0x01), or 2 (0x02)
        */
        if( (will_qos >= 0) && (will_qos <= 2) )
        {
            LOG_ERROR("[MQTT-3.1.2-12] bad will qos value");
            return 0;
        }       
    }

    /* Pass connect flags field */
    packet_index += 1;

    keep_alive = (packet[packet_index] << 8) + packet[packet_index + 1];

    /* Pass keep alive field */
    packet_index += 2;

    /* Parse CONNECT Properties */
    if( protocol_version == 5 )
    {
        if( decode_variable_byte(packet, packet_index, &properties_field_length) != 0 )
        {
            LOG_ERROR("failed to parse property length Variable Byte");
            return 0;
        }

        /* Pass properties length field */
        packet_index += 1;

        for( i=0; i<properties_field_length; )
        {
            properties_field_name = packet[packet_index];

            /* Pass property field name */
            packet_index += 1;
            i += 1;

            switch(properties_field_name)
            {
                case PROP_SESSION_EXPIRY_INTERVAL:
                    packet_index += 4;
                    i += 4;
                    break;

                case PROP_RECEIVE_MAXIMUM:
                    packet_index += 2;
                    i += 2;
                    break;

                case PROP_MAXIMUM_PACKET_SIZE:
                    packet_index += 4;
                    i += 4;
                    break;

                case PROP_TOPIC_ALIAS_MAXIMUM:
                    packet_index += 2;
                    i += 2;
                    break;

                case PROP_REQUEST_RESPONSE_INFORMATION:
                    packet_index += 1;
                    i += 1;
                    break;

                case PROP_REQUEST_PROBLEM_INFORMATION:
                    packet_index += 1;
                    i += 1;
                    break;

                case PROP_USER_PROPERTY:
                    /* TODO: parse String Pair */
                    break;

                case PROP_AUTHENTICATION_METHOD:
                    /* TODO: parse Binary Data */
                    break;

                default:
                    LOG_ERROR("unknown property field");
                    return 0;
                    break;
            }
        }
    }

    payload_field_length = (packet[packet_index] << 8) + packet[packet_index + 1];

    /* Pass payload length field */
    packet_index += 2;

    /* [MQTT-3.1.3-5]
        The Server MUST allow ClientID’s which are between 1 and 23 UTF-8 encoded bytes in length, 
        and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    */
    if( payload_field_length > 23 )
    {
        LOG_ERROR("[MQTT-3.1.3-5] clientID must be < 23");
        return 0;
    }

    char c = 0;
    for( i=0; i < payload_field_length; i++)
    {
        c = packet[packet_index + i];
        if( ((c > '0') && (c < '9')) || ((c > 'a') && (c < 'z')) || ((c > 'A') && (c < 'Z')) )
        {
            client_id[i] = packet[packet_index + i];
        }
        else
        {
            LOG_ERROR("[MQTT-3.1.3-5] clientID must only contain characters");
            return 0;
        }
    }
    client_id[i + 1] = 0;

    /* Pass clientID field */
    packet_index += payload_field_length;

    if( will_flag == 1 )
    {
        if( protocol_version == 5 )
        {
            if( decode_variable_byte(packet, packet_index, &will_properties_field_length) != 0 )
            {
                LOG_ERROR("failed to parse will property length Variable Byte");
                return 0;
            }

            /* Pass properties length field */
            packet_index += 1;

            for( i=0; i<will_properties_field_length; )
            {
                will_properties_field_name = packet[packet_index];

                /* Pass property field name */
                packet_index += 1;
                i += 1;

                switch(properties_field_name)
                {
                    case PROP_WILL_DELAY_INTERVAL:
                        packet_index += 4;
                        i += 4;
                        break;

                    case PROP_PAYLOAD_FORMAT_INDICATOR:
                        packet_index += 1;
                        i += 1;
                        break;

                    case PROP_MESSAGE_EXPIRY_INTERVAL:
                        packet_index += 4;
                        i += 4;
                        break;

                    case PROP_CONTENT_TYPE:
                        /* TODO: parse UTF-8 string */
                        break;

                    case PROP_RESPONSE_TOPIC:
                        /* TODO: parse UTF-8 string */
                        break;

                    case PROP_CORRELATION_DATA:
                        /* TODO: parse Binary Data */
                        break;

                    case PROP_USER_PROPERTY:
                        /* TODO: parse String Pair */
                        break;
                }
            }
        }

        /* TODO: parse will topic */
        /* TODO: parse will payload */
    }

    if( username_flag == 1 )
    {
        payload_field_length = (packet[packet_index] << 8) + packet[packet_index + 1];

        /* Pass payload length field */
        packet_index += 2;

        for( i=0; i < payload_field_length; i++)
        {
            username[i] = packet[14 + i];
        }

        /* Pass username field */
        packet_index += payload_field_length;
    }

    if( password_flag == 1 )
    {
        payload_field_length = (packet[packet_index] << 8) + packet[packet_index + 1];

        /* Pass payload length field */
        packet_index += 2;

        for( i=0; i < payload_field_length; i++)
        {
            password[i] = packet[packet_index + i];
        }

        /* Pass password field */
        packet_index += payload_field_length;
    }

    if( packet_index != length )
    {
        LOG_ERROR("Extra bytes in the connect packet");
        return 0;
    }

    LOG_DEBUG("<- CONNACK");
    packets_encode_connack(fd);

    return 1;
}
