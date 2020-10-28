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
 * @brief Protocol definition for messages in audio control headers. This
 *        can be used by the primary host computer to control and exchange
 *        audio, midi and other data with secondary microcontrollers.
 * @copyright 2018-2020 Modern Ancient Instruments Networked AB, dba Elk, Stockholm
 */
#ifndef AUDIO_PROTOCOL_H_
#define AUDIO_PROTOCOL_H_

#include "audio_protocol_common.h"

#ifdef __cplusplus
namespace audio_ctrl {
#endif

// The max number of bytes which the protocol can carry as payload.
#define AUDIO_CTRL_PKT_PAYLOAD_SIZE 128

// Max number of input and output cv gates that this protocol supports
#define AUDIO_CTRL_PKT_MAX_NUM_CV_IN_GATES 16
#define AUDIO_CTRL_PKT_MAX_NUM_CV_OUT_GATES 16

// Gpio Data Payload size.
#define AUDIO_CTRL_PKT_GPIO_DATA_BLOB_SIZE 32
#define AUDIO_CTRL_PKT_GPIO_DATA_BLOB_SIZE_WORDS (AUDIO_CTRL_PKT_GPIO_DATA_SIZE / 4)
#define AUDIO_CTRL_PKT_MAX_NUM_GPIO_DATA_BLOBS (AUDIO_CTRL_PKT_PAYLOAD_SIZE / AUDIO_CTRL_PKT_GPIO_DATA_BLOB_SIZE)

// Hardcoded size definitions
#define AUDIO_CTRL_PKT_SIZE 152
#define AUDIO_CTRL_PKT_SIZE_WORDS 38

// stucture to represent gpio data
struct GpioDataBlob
{
    uint8_t data[AUDIO_CTRL_PKT_GPIO_DATA_BLOB_SIZE];
};

// Union representing the payloads the audio control protocol can carry
union AudioPacketPayload
{
    uint8_t midi_data[AUDIO_CTRL_PKT_PAYLOAD_SIZE];
    struct GpioDataBlob gpio_data_blob[AUDIO_CTRL_PKT_MAX_NUM_GPIO_DATA_BLOBS];
};

/**
 * Command codes (MSB)
 */
typedef enum
{
    AUDIO_CMD_NULL = 0,
    AUDIO_CMD_MUTE = 100,
    AUDIO_CMD_UNMUTE = 101,
    AUDIO_CMD_CEASE = 102,
    GPIO_DATA = 179,
    MIDI_DATA = 186
} AudioCtrlCmds;

/**
 * Packet definition
 */
typedef struct
{
    // magic start chars 'm', 'd'
    uint8_t     magic_start[2];

    // command msb & lsb
    uint8_t     cmd_msb;
    uint8_t     cmd_lsb;

    // command payload
    union       AudioPacketPayload payload;

    // Sequential packet number
    uint32_t    seq;

    // timing error between xmos and audio host
    int32_t     timing_error;

    // contains cv gate in data, each bit represents a gate
    uint32_t    cv_gate_in;

    // contains cv gate out data, each bit represents a gate
    uint32_t    cv_gate_out;

    // N. of packets remaining in current message
    uint8_t     continuation;

    // magic stop char 'z'
    uint8_t     magic_stop;

    // Poor's man CRC
    uint16_t    crc;
} AudioCtrlPkt;

// statically verify the hardcoded size definitions
COMPILER_VERIFY(sizeof(AudioCtrlPkt) == AUDIO_CTRL_PKT_SIZE);
COMPILER_VERIFY(sizeof(AudioCtrlPkt)/4 == AUDIO_CTRL_PKT_SIZE_WORDS);
COMPILER_VERIFY(sizeof(union AudioPacketPayload) == 128);
COMPILER_VERIFY((sizeof(struct GpioDataBlob) * AUDIO_CTRL_PKT_MAX_NUM_GPIO_DATA_BLOBS) == sizeof(union AudioPacketPayload));

#ifdef __cplusplus
} // namespace audio_ctrl
#endif

#endif /* AUDIO_PROTOCOL_H_ */