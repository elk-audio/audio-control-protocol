/*
 * Copyright 2018-2020 Modern Ancient Instruments Networked AB, dba Elk
 * Audio Control Protocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Audio Control Protocol is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Audio Control Protocol. If not, see http://www.gnu.org/licenses/ .
 */

/**
 * @brief Helper functions to parse or generate audio control packets. Can be
 *        used either by the host system or the secondary microcontroller.
 * @copyright 2018-2020 Modern Ancient Instruments Networked AB, dba Elk, Stockholm
 */
#ifndef AUDIO_PACKET_HELPER_
#define AUDIO_PACKET_HELPER_

#include "audio_control_protocol.h"

#ifdef __cplusplus
namespace audio_ctrl {
#endif

/**
 * @brief Clears the audio packet
 *
 * @param pkt The audio control packet
 */
#ifdef __XC__
#pragma unsafe arrays
#pragma loop unroll
#endif
inline void clear_audio_ctrl_pkt(AudioCtrlPkt* const pkt)
{
    uint32_t* pkt_data = (uint32_t*) pkt;
    for (uint32_t i = 0; i < AUDIO_CTRL_PKT_SIZE_WORDS; i++)
    {
        pkt_data[i] = 0;
    }
}

/**
 * @brief Creates a default audio control packet.
 *
 * @param pkt the audio control packet
 */
inline void create_default_audio_ctrl_pkt(AudioCtrlPkt* const pkt)
{
    clear_audio_ctrl_pkt(pkt);
    pkt->magic_start[0] = 'm';
    pkt->magic_start[1] = 'd';
    pkt->magic_stop = 'z';
}

/**
 * @brief checks for the presence of magic words in the packet
 *
 * @param pkt the audio control packet
 * @return 1 if packet contains magic words, 0 if not
 */
inline int check_audio_pkt_for_magic_words(const AudioCtrlPkt* const pkt)
{
    if (pkt->magic_start[0] != 'm' ||
        pkt->magic_start[1] != 'd' ||
        pkt->magic_stop != 'z')
    {
        return 0;
    }

    return 1;
}

/**
 * @brief Checks if packet has audio mute command
 *
 * @param pkt the audio control packet
 * @return 1 if packet has audio mute command, 0 otherwise
 */
int inline check_for_audio_mute_cmd(const AudioCtrlPkt* const pkt)
{
    if (pkt->cmd_msb == AUDIO_CMD_MUTE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio mute packet
 *
 * @param pkt the audio control packet
 * @param seq_number the packet's sequence number
 */
inline void prepare_audio_mute_pkt(AudioCtrlPkt* const pkt,
                                   uint32_t seq_number)
{
    create_default_audio_ctrl_pkt(pkt);
    pkt->cmd_msb = AUDIO_CMD_MUTE;
    pkt->seq = seq_number;
}

/**
 * @brief Checks if packet has audio unmute command
 *
 * @param pkt the audio control packet
 * @return 1 if packet has audio mute command, 0 otherwise
 */
inline int check_for_audio_unmute_cmd(const AudioCtrlPkt* const pkt)
{
    if (pkt->cmd_msb == AUDIO_CMD_UNMUTE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio unmute packet
 *
 * @param pkt the audio control packet
 * @param seq_number the packet's sequence number
 */
inline void prepare_audio_unmute_pkt(AudioCtrlPkt* const pkt,
                                     uint32_t seq_number)
{
    create_default_audio_ctrl_pkt(pkt);
    pkt->cmd_msb = AUDIO_CMD_UNMUTE;
    pkt->seq = seq_number;
}

/**
 * @brief Check for an cease audio command.
 *
 * @param pkt The audio control packet
 * @return 1 if packet has an audio cease command, 0 if not.
 */
inline int check_for_audio_cease_cmd(const AudioCtrlPkt* const pkt)
{
    if (pkt->cmd_msb == AUDIO_CMD_CEASE)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief prepares an audio cease packet.
 *
 * @param pkt the audio control packet
 * @param the packet's sequence number
 */
inline void prepare_audio_cease_pkt(AudioCtrlPkt* const pkt,
                                    uint32_t seq_number)
{
    create_default_audio_ctrl_pkt(pkt);
    pkt->cmd_msb = AUDIO_CMD_CEASE;
    pkt->seq = seq_number;
}

/**
 * @brief Checks for gpio data in the payload
 *
 * @param pkt The audio control packet
 * @return int The number of gpio data blobs in the payload
 */
inline int check_for_gpio_data(const AudioCtrlPkt* const pkt)
{
    if (pkt->cmd_msb == GPIO_DATA)
    {
        // contains how many gpio data blobs are there in the payload
        return pkt->cmd_lsb;
    }

    return 0;
}

/**
 * @brief prepares an gpio data packet. The number of gpio data blobs the
 *        packet can hold is defined by AUDIO_CTRL_PKT_MAX_NUM_GPIO_DATA_BLOBS.
 *        Note that, unlike the other functions, this is not responsible for
 *        clearing the packet as well as inserting the gpio packets into the
 *        payload. Ideally, this should be called after such work is done.
 *
 * @param pkt the audio control packet
 * @param num_gpio_data_blobs The number of gpio data blobs this audio packet
 *                            will hold in its payload
 * @return -1 if num_gpio_data_blobs if greater than what the paylaod can hold,
 *          0 otherwise.
 */
inline int prepare_gpio_cmd_pkt(AudioCtrlPkt* const pkt,
                                uint8_t num_gpio_data_blobs)
{
    #ifdef DEBUG
        if (num_gpio_data_blobs > AUDIO_CTRL_PKT_MAX_NUM_GPIO_DATA_BLOBS)
        {
            return -1;
        }
    #endif

    pkt->cmd_msb = GPIO_DATA;
    pkt->cmd_lsb = num_gpio_data_blobs;

    return 0;
}

/**
 * @brief Prepare a packet with midi data as payload
 *
 * @param pkt The audio control packet
 * @param midi_data The midi data
 * @param midi_data_size The size of midi data. It should be lesser than
 *        AUDIO_CONTROL_PACKET_PAYLOAD_SIZE.
 * @return 0 indicates an error ie midi_data_size is bigger than
 *         AUDIO_CONTROL_PACKET_PAYLOAD_SIZE, 1 if successful
 */
inline int prepare_midi_data_pkt(AudioCtrlPkt* const pkt,
                                 const uint8_t* const midi_data,
                                 uint8_t num_midi_bytes)
{
    #ifdef DEBUG
    if (num_midi_bytes > AUDIO_CTRL_PKT_PAYLOAD_SIZE)
    {
        return 0;
    }
    #endif

    create_default_audio_ctrl_pkt(pkt);
    pkt->cmd_msb = MIDI_DATA;
    pkt->cmd_lsb = num_midi_bytes;
    for (int i = 0; i < (int) num_midi_bytes; i++)
    {
        pkt->payload.midi_data[i] = midi_data[i];
    }

    return 1;
}

/**
 * @brief Check for midi data in the packet.
 * @param pkt The audio control packet
 * @return The number of midi bytes if the packet contains midi data, 0 if not
 */
inline int check_for_midi_data(const AudioCtrlPkt* const pkt)
{
    if (pkt->cmd_msb == MIDI_DATA)
    {
        return pkt->cmd_lsb;
    }

    return 0;
}

/**
 * @brief Get midi data from packet and store into a dest buffer
 * @param pkt The audio control packet
 * @param dest_midi_buffer The destination buffer to store the midi data
 * @param offset An offset in the payload data can be specified from which a
 *               section of midi data can be retrieved.
 * @param num_midi_bytes The number of midi bytes to be retrieved.
 * @return
 */
#ifdef __XC__
#pragma unsafe arrays
#endif
inline int get_midi_data(const AudioCtrlPkt* const pkt,
                         uint8_t* const dest_midi_buffer,
                         int offset,
                         int num_midi_bytes)
{
    #ifdef DEBUG
    if (offset + num_midi_bytes > AUDIO_CTRL_PKT_PAYLOAD_SIZE)
    {
        return 0;
    }
    #endif

    const uint8_t* midi_data = &pkt->payload.midi_data[offset];
    for (int i = 0; i < num_midi_bytes; i++)
    {
        dest_midi_buffer[i] = midi_data[i];
    }

    return 1;
}

/**
 * @brief Gets the timing error info from the audio control packet.
 *
 * @param pkt the audio control packet
 * @return The timing error.
 */
inline int32_t get_timing_error(const AudioCtrlPkt* const pkt)
{
    return pkt->timing_error;
}

/**
 * @brief Sets the timing error.
 *
 * @param pkt the audio control packet
 * @param timing_error The timing error
 */
inline void set_timing_error(AudioCtrlPkt* const pkt,
                             int32_t timing_error)
{
    pkt->timing_error = timing_error;
}

/**
 * @brief Set the cv gate out val
 *
 * @param pkt The audio control packet
 * @param cv_gate_out_val The value of all the gates, where each bit represents
 *        the value of an individual gate.
 */
inline void set_cv_gate_out_val(AudioCtrlPkt* const pkt,
                                uint32_t cv_gate_out_val)
{
    pkt->cv_gate_out = cv_gate_out_val;
}

/**
 * @brief Get the cv gate in val from the packet
 *
 * @param pkt The audio control packet
 * @return uint32_t input CV gate value where each bit represents the value of
 *         one gate
 */
inline uint32_t get_cv_gate_in_val(AudioCtrlPkt* const pkt)
{
    return pkt->cv_gate_in;
}

#ifdef __cplusplus
} // namespace audio_ctrl
#endif

#endif