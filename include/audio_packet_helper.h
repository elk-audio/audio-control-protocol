/**
 * @file helper functions to parse or generate audio control packets.
 * @copyright MIND Music Labs AB, Stockholm
 */
#ifndef AUDIO_PACKET_HELPER_
#define AUDIO_PACKET_HELPER_

#include "audio_control_protocol.h"

#ifdef __cplusplus__
extern "C" {
#endif

/**
 * @brief Creates a default audio control packet.
 *
 * @param packet the audio control packet
 */
void create_default_audio_control_packet(AudioControlPacket* packet);

/**
 * @brief checks for the precence of magic words in the packet
 *
 * @param packet the audio control packet
 * @return 1 if packet contains magic words, 0 if not
 */
int check_audio_packet_for_magic_words(const AudioControlPacket* packet);

/**
 * @brief Gets the command in an audio packet.
 *
 * @param packet the audio control packet
 * @return The audio packet command.
 */
uint8_t get_audio_packet_cmd(const AudioControlPacket* packet);

/**
 * @brief prepares an audio mute packet
 *
 * @param the audio control packet
 */
void prepare_audio_mute_packet(AudioControlPacket* packet);

/**
 * @brief prepares an audio unmute packet
 *
 * @param packet the audio control packet
 */
void prepare_audio_unmute_packet(AudioControlPacket* packet);

/**
 * @brief prepares an gpio cmd packet
 *
 * @param packet the audio control packet
 * @param gpio_packet_data pointer to the gpio packet data that becomes this
 *        packets payload.
 */
void prepare_gpio_cmd_packet(AudioControlPacket* packet,
                             const uint8_t* gpio_packet_data);

/**
 * @brief Gets the gpio packet data from the payload, assuming it contains it in
 *        the first place.
 *
 * @param packet the audio control packet
 * @param gpio_packet_data pointer to the location where the gpio data will be
 *        filled after being retreived from the payload.
 */
void get_gpio_packet_data(const AudioControlPacket* packet,
                          uint8_t* gpio_packet_data);

/**
 * @brief Gets the timing error.
 *
 * @param packet the audio control packet
 * @return The timing error.
 */
int32_t get_timing_error(const AudioControlPacket* packet);

/**
 * @brief Sets the timing error.
 *
 * @param packet the audio control packet
 * @param timing_error The timing error
 */
void set_timing_error(AudioControlPacket* packet, int32_t timing_error);

/**
 * @brief resets the internal sequence counter
 */
void reset_sequence_counter();

#ifdef __cplusplus__
} // extern C
#endif

#endif