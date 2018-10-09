/**
 * audio_protocol.h
 *
 * @brief Protocol definition for messages in control headers
 */
#ifndef AUDIO_PROTOCOL_H_
#define AUDIO_PROTOCOL_H_

#ifndef __KERNEL__
#include <stdint.h>
#include <stddef.h>
#endif

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
} AudioVersionData;

/**
 * Data structure to denote the payload elements associated with START_COMMAND
 */
typedef struct StartCmdData
{
    uint32_t buffer_size;
} StartCmdData;

/**
 * A union for the data payload of the control packet.
 */
typedef union DataPayload
{
    StartCmdData start_cmd_data;
    AudioVersionData version_data;
    uint8_t raw_data[AUDIO_CONTROL_PACKET_PAYLOAD_SIZE];
} DataPayload;

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
    DataPayload  data_payload;
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

#ifdef __cplusplus
constexpr uint32_t AUDIO_CONTROL_PACKET_SIZE = sizeof(AudioControlPacket);
static_assert(AUDIO_CONTROL_PACKET_SIZE == 48);
}
#endif

#endif /* AUDIO_PROTOCOL_H_ */