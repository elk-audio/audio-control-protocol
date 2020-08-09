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
 * @brief Clears the audio packet
 *
 * @param packet The audio control packet
 */
inline void clear_audio_control_packet(AudioControlPacket* const packet)
{
    uint32_t* ptr = (uint32_t*) packet;
    for(uint32_t i = 0; i < AUDIO_CONTROL_PACKET_SIZE_WORDS; i++)
    {
        ptr[i] = 0;
    }
}

/**
 * @brief Creates a default audio control packet.
 *
 * @param packet the audio control packet
 */
inline void create_default_audio_control_packet(AudioControlPacket* const
                                                packet)
{
    clear_audio_control_packet(packet);
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
inline int check_audio_packet_for_magic_words(const AudioControlPacket* const
                                              packet)
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
 * @brief Checks if packet has audio mute command
 *
 * @param packet the audio control packet
 * @return 1 if packet has audio mute command, 0 otherwise
 */
int inline check_for_audio_mute_cmd(const AudioControlPacket* const packet)
{
    if(packet->cmd_msb == AUDIO_CMD_MUTE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio mute packet
 *
 * @param the audio control packet
 * @param the packet's sequence number
 */
inline void prepare_audio_mute_packet(AudioControlPacket* const packet,
                                      uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_MUTE;
    packet->seq = seq_number;
}

/**
 * @brief Checks if packet has audio unmute command
 *
 * @param packet the audio control packet
 * @return 1 if packet has audio mute command, 0 otherwise
 */
inline int check_for_audio_unmute_cmd(const AudioControlPacket* const packet)
{
    if(packet->cmd_msb == AUDIO_CMD_UNMUTE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio unmute packet
 *
 * @param packet the audio control packet
 * @param the packet's sequence number
 */
inline void prepare_audio_unmute_packet(AudioControlPacket* const packet,
                                        uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_UNMUTE;
    packet->seq = seq_number;
}

/**
 * @brief Check for an cease audio command. This is always followed by a stop
 *        command from the device control protocol. and acts as a precursor so
 *        that the device can prepare to reset its states
 *
 * @param packet The audio control packet
 * @return 1 if packet has an audio cease command, 0 if not.
 */
inline int check_for_audio_cease_cmd(const AudioControlPacket* const packet)
{
    if(packet->cmd_msb == AUDIO_CMD_CEASE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio cease packet. This is always followed by a stop
 *        command from the device control protocol. and acts as a precursor so
 *        that the device can prepare to reset its states
 *
 * @param packet the audio control packet
 * @param the packet's sequence number
 */
inline void prepare_audio_cease_packet(AudioControlPacket* const packet,
                                       uint32_t seq_number)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_CEASE;
    packet->seq = seq_number;
}

/**
 * @brief Checks for gpio packet in the payload
 *
 * @param packet The audio control packet
 * @return int The number of gpio packets in the payload
 */
inline int check_for_gpio_packet(const AudioControlPacket* const packet)
{
    if(packet->cmd_msb == GPIO_PACKET)
    {
        // contains how many gpio packets are there in the payload
        return packet->cmd_lsb;
    }

    return 0;
}

/**
 * @brief Get the gpio packet data from the payload and store it into a buffer
 *
 * @param packet The audio control packet
 * @param packet The buffer where the data is to be copied into
 * @param packet_number Which packet is to be retreived in the payload. It
 *                      ranges from 0 -> MAX_NUM_GPIO_PACKETS - 1 and is
 *                      dependent on how many packets are there in the payload,
 *                      which is given by the cmd_lsb.
 * @return 0 if the packet number does not exist in the payload. 1 if
 *         successful.
 */
inline int get_gpio_packet(const AudioControlPacket* const packet,
                            uint8_t* const gpio_packet_data,
                            int gpio_packet_num)
{
    #ifdef DEBUG
    // Check if the gpio packet being retrieved exists in the payload
    if(gpio_packet_num >= cmd_lsb)
    {
        return 0;
    }
    #endif

    const uint8_t* packet_data = packet->payload + (gpio_packet_num * GPIO_PACKET_SIZE);
    for(int i = 0; i < GPIO_PACKET_SIZE; i++)
    {
        gpio_packet_data[i] = packet_data[i];
    }

    return 1;
}

/**
 * @brief prepares an gpio cmd packet
 *
 * @param packet the audio control packet
 * @param gpio_packet_data pointer to the gpio packet data that becomes this
 *        packets payload.
 * @param gpio_packet_size The size of the gpio packet.
 * @param seq_number the packet's sequence number
 * @return 0 indicates an error ie the number of packets is too much for the
 *         payload size. 1 indicates a success.
 */
inline int prepare_gpio_cmd_packet(AudioControlPacket* const packet,
                                   const uint8_t* const gpio_packet_data,
                                   uint8_t num_gpio_packets,
                                   uint32_t seq_number)
{
    #ifdef DEBUG
    if(num_gpio_packets > MAX_NUM_GPIO_PACKETS)
    {
        return 0
    }
    #endif

    create_default_audio_control_packet(packet);
    packet->cmd_msb = GPIO_PACKET;
    packet->cmd_lsb = num_gpio_packets;
    for(int i = 0; i < (num_gpio_packets * GPIO_PACKET_SIZE); i++)
    {
        packet->payload[i] = gpio_packet_data[i];
    }
    packet->seq = seq_number;

    return 1;
}

/**
 * @brief Prepare a packet with midi data as payload
 *
 * @param packet The midi packet
 * @param midi_data The midi data
 * @param midi_data_size The size of midi data. It should be lesser than
 *        AUDIO_CONTROL_PACKET_PAYLOAD_SIZE.
 * @return 0 indicates an error ie midi_data_size is bigger than
 *         AUDIO_CONTROL_PACKET_PAYLOAD_SIZE, 1 if successful
 */
inline int prepare_midi_data_packet(AudioControlPacket* const packet,
                                    const uint8_t* const midi_data,
                                    uint8_t num_midi_bytes)
{
    #ifdef DEBUG
    if(midi_data_size > AUDIO_CONTROL_PACKET_PAYLOAD_SIZE)
    {
        return 0
    }
    #endif

    create_default_audio_control_packet(packet);
    packet->cmd_msb = MIDI_PACKET;
    packet->cmd_lsb = num_midi_bytes;
    for(uint8_t i = 0; i < num_midi_bytes; i++)
    {
        packet->payload[i] = midi_data[i];
    }

    return 1;
}

/**
 * @brief Check for midi data in the packet.
 * @param packet The audio control packet
 * @return The number of midi bytes if the packet contains midi data, 0 if not
 */
inline int check_for_midi_data(const AudioControlPacket* const packet)
{
    if(packet->cmd_msb == MIDI_PACKET)
    {
        return packet->cmd_lsb;
    }

    return 0;
}

/**
 * @brief Get midi data from packet and store into a buffer
 * @param packet The audio control packet
 * @param dest_midi_buffer The destination buffer to store the midi data
 * @param offset An offset in the payload data can be specified from which a
 *               section of midi data can be retrieved.
 * @param num_midi_bytes The number of midi bytes to be retrieved.
 * @return
 */
inline int get_midi_data(const AudioControlPacket* const packet,
                         uint8_t* const dest_midi_buffer,
                         int offset,
                         int num_midi_bytes)
{
    #ifdef DEBUG
    if(start_offset + num_midi_bytes > AUDIO_CONTROL_PACKET_PAYLOAD_SIZE)
    {
        return 0;
    }
    #endif

    const uint8_t* midi_data = packet->payload + offset;
    for(int i = 0; i < num_midi_bytes; i++)
    {
        dest_midi_buffer[i] = midi_data[i];
    }

    return 1;
}

/**
 * @brief Gets the timing error.
 *
 * @param packet the audio control packet
 * @return The timing error.
 */
inline int32_t get_timing_error(const AudioControlPacket* const packet)
{
    return packet->timing_error;
}

/**
 * @brief Sets the timing error.
 *
 * @param packet the audio control packet
 * @param timing_error The timing error
 */
inline void set_timing_error(AudioControlPacket* const packet,
                             int32_t timing_error)
{
    packet->timing_error = timing_error;
}

inline void set_cv_gate_out_val(AudioControlPacket* const packet,
                                uint32_t cv_gate_out_val)
{
    packet->cv_gate_out = cv_gate_out_val;
}

inline uint32_t get_cv_gate_in_val(AudioControlPacket* const packet)
{
    return packet->cv_gate_in;
}

#ifdef __cplusplus
} // extern C
#endif

#endif