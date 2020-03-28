#include "packets_decoder.h"

int decode_variable_byte(uint8_t *buffer, size_t index, uint32_t *value)
{
    uint8_t multiplier = 1;
    uint8_t encodedByte;
    *value = 0;

    do
    {
        encodedByte = buffer[index];
        *value += (encodedByte & 127) * multiplier;
        if( multiplier > 128*128*128 )
        {
            return -1;
        }
        multiplier *= 128;
        index++;
    } while( (encodedByte & 128) != 0 );

    return 0;
}