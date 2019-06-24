/**
 * @file helper functions to parse or generate audio control packets.
 * @copyright MIND Music Labs AB, Stockholm
 */
#ifndef AUDIO_PACKET_HELPER_
#define AUDIO_PACKET_HELPER_

#include <string.h>

#include "audio_control_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a default audio control packet.
 *
 * @param packet the audio control packet
 */
static inline void create_default_audio_control_packet(volatile AudioControlPacket* packet)
{
    volatile int* ptr = (int*)packet;
    for(int i = 0; i < AUDIO_CONTROL_PACKET_SIZE/4; i++)
    {
        ptr[i] = 0;
    }
    packet->magic_start[0] = 'm';
    packet->magic_start[1] = 'd';
    packet->magic_stop = 'z';
}

/**
 * @brief checks for the precence of magic words in the packet
 *
 * @param packet the audio control packet
 * @return 1 if packet contains magic words, 0 if not
 */
static inline int check_audio_packet_for_magic_words(volatile const AudioControlPacket* packet)
{
    if(packet->magic_start[0] != 'm' ||
       packet->magic_start[1] != 'd' ||
       packet->magic_stop != 'z')
    {
        return 0;
    }

    return 1;
}

/**
 * @brief Gets the command in an audio packet.
 *
 * @param packet the audio control packet
 * @return The audio packet command.
 */
static inline uint8_t get_audio_packet_cmd(volatile const AudioControlPacket* packet)
{
    return packet->cmd_msb;
}

/**
 * @brief prepares an audio mute packet
 *
 * @param the audio control packet
 * @param the packet's sequence number
 */
static inline void prepare_audio_mute_packet(volatile AudioControlPacket* packet,
                                             uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_MUTE;
    packet->seq = seq_number;
}

/**
 * @brief prepares an audio unmute packet
 *
 * @param packet the audio control packet
 * @param the packet's sequence number
 */
static inline void prepare_audio_unmute_packet(volatile AudioControlPacket* packet,
                                               uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_UNMUTE;
    packet->seq = seq_number;
}

/**
 * @brief prepares an gpio cmd packet
 *
 * @param packet the audio control packet
 * @param gpio_packet_data pointer to the gpio packet data that becomes this
 *        packets payload.
 * @param the packet's sequence number
 */
static inline void prepare_gpio_cmd_packet(volatile AudioControlPacket* packet,
                                           const uint8_t* gpio_packet_data,
                                           uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = GPIO_PACKET;
    for(int i = 0; i < GPIO_PACKET_SIZE; i++)
    {
        packet->payload[i] = gpio_packet_data[i];
    }
    packet->seq = seq_number;
}

/**
 * @brief Gets the gpio packet data from the payload, assuming it contains it in
 *        the first place.
 *
 * @param packet the audio control packet
 * @param gpio_packet_data pointer to the location where the gpio data will be
 *        filled after being retreived from the payload.
 */
static inline void get_gpio_packet_data(volatile const AudioControlPacket* packet,
                                        uint8_t* gpio_packet_data)
{
    for(int i = 0; i < GPIO_PACKET_SIZE; i++)
    {
        gpio_packet_data[i] = packet->payload[i];
    }
}

/**
 * @brief Gets the timing error.
 *
 * @param packet the audio control packet
 * @return The timing error.
 */
static inline int32_t get_timing_error(volatile const AudioControlPacket* packet)
{
    return packet->timing_error;
}

/**
 * @brief Sets the timing error.
 *
 * @param packet the audio control packet
 * @param timing_error The timing error
 */
static inline void set_timing_error(volatile AudioControlPacket* packet, int32_t timing_error)
{
    packet->timing_error = timing_error;
}


#ifdef __cplusplus
} // extern C
#endif

#endif