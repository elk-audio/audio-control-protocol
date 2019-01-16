/**
 * @brief Protocol definition for messages in control headers
 * @copyright MIND Music Labs AB, Stockholm
 */
#ifndef AUDIO_PROTOCOL_H_
#define AUDIO_PROTOCOL_H_

#ifndef __KERNEL__
#include <stdint.h>
#include <stddef.h>
#endif

#define AUDIO_CONTROL_PACKET_PAYLOAD_SIZE 32
#define GPIO_PACKET_SIZE AUDIO_CONTROL_PACKET_PAYLOAD_SIZE

/**
 * Command codes (MSB)
 */
typedef enum
{
    AUDIO_CMD_NULL = 0,
    AUDIO_CMD_MUTE = 100,
    AUDIO_CMD_UNMUTE = 101,
    GPIO_PACKET = 179
} AudioControlCommands;

/**
 * Packet definition
 */
typedef struct
{
    //@ magic start chars 'm', 'd'
    uint8_t     magic_start[2];
    //@ command msb & lsb
    uint8_t     cmd_msb;
    uint8_t     cmd_lsb;
    //@ command payload
    uint8_t     payload[AUDIO_CONTROL_PACKET_PAYLOAD_SIZE];
    //@ Sequential packet number
    uint32_t    seq;
    //@ timing error between xmos and audio host
    int32_t     timing_error;
    //@ N. of packets remaining in current message
    uint8_t     continuation;
    //@ magic stop char 'z'
    uint8_t     magic_stop;
    //@ Poor's man CRC
    uint16_t    crc;
} __attribute__((packed)) AudioControlPacket;

#define AUDIO_CONTROL_PACKET_SIZE sizeof(AudioControlPacket)

#endif /* AUDIO_PROTOCOL_H_ */