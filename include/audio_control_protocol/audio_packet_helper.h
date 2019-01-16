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
inline void create_default_audio_control_packet(AudioControlPacket* packet)
{
    memset(packet, 0, AUDIO_CONTROL_PACKET_SIZE);
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
inline int check_audio_packet_for_magic_words(const AudioControlPacket* packet)
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
inline uint8_t get_audio_packet_cmd(const AudioControlPacket* packet)
{
    return packet->cmd_msb;
}

/**
 * @brief prepares an audio mute packet
 *
 * @param the audio control packet
 * @param the packet's sequence number
 */
inline void prepare_audio_mute_packet(AudioControlPacket* packet,
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
inline void prepare_audio_unmute_packet(AudioControlPacket* packet,
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
inline void prepare_gpio_cmd_packet(AudioControlPacket* packet,
                                    const uint8_t* gpio_packet_data,
                                    uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = GPIO_PACKET;
    memcpy(packet->payload, gpio_packet_data, GPIO_PACKET_SIZE);
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
inline void get_gpio_packet_data(const AudioControlPacket* packet,
                                 uint8_t* gpio_packet_data)
{
    memcpy(gpio_packet_data, packet->payload, GPIO_PACKET_SIZE);
}

/**
 * @brief Gets the timing error.
 *
 * @param packet the audio control packet
 * @return The timing error.
 */
inline int32_t get_timing_error(const AudioControlPacket* packet)
{
    return packet->timing_error;
}

/**
 * @brief Sets the timing error.
 *
 * @param packet the audio control packet
 * @param timing_error The timing error
 */
inline void set_timing_error(AudioControlPacket* packet, int32_t timing_error)
{
    packet->timing_error = timing_error;
}


#ifdef __cplusplus
} // extern C
#endif

#endif