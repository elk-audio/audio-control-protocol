/**
 * audio_protocol.h
 *
 * @brief Protocol definition for messages in control headers
 */
#ifndef AUDIO_PROTOCOL_H_
#define AUDIO_PROTOCOL_H_

#include <stdint.h>
#include <string.h>
#include <stddef.h>

/**
 * Constants
 */
#define AUDIO_CONTROL_PACKET_PAYLOAD_SIZE 32

#ifdef __cplusplus
#include <cassert>
namespace AudioProtocol {
#endif

/**
 * Command codes (MSB)
 */
typedef enum
{
    AUDIO_CMD_NULL = 0,
    AUDIO_CMD_START = 123,
    AUDIO_CMD_STOP = 234,
    AUDIO_CMD_MUTE = 100,
    SENSEI_PACKET = 179,
    AUDIO_FIRMWARE_VER_CHECK = 191
} AudioControlCommands;

/**
 * Packet definition for audio system version
 */
typedef struct
{
    uint32_t    major;
    uint32_t    minor;
    uint32_t    revision;
} AudioVersionPacket;

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
    uint8_t     data[AUDIO_CONTROL_PAYLOAD_SIZE];
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

/**
 * Static variables
 */

/**
 * Default Control packet definition
 */
static const  AudioControlPacket _AUDIO_DEFAULT_CONTROL_PACKET =
{
    .magic_start = {'m', 'd'},
    .cmd_msb = 0,
    .cmd_lsb = 0,
    .data = {0},
    .seq = 0,
    .timing_error = 0,
    .continuation = 0,
    .crc = 0,
    .magic_stop = 'z'
};

#ifdef __cplusplus
static_assert(sizeof(AudioControlPacket) == 48);
}
#endif

#endif /* AUDIO_PROTOCOL_H_ */