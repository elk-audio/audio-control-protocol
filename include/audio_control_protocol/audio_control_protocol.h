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

// The max number of bytes which the protocol can carry as payload.
#define AUDIO_CONTROL_PACKET_PAYLOAD_SIZE 128

// The size of a gpio packet
#define GPIO_PACKET_SIZE 32

// Max number of gpio packets that the protocol can carry (payload / gpio packet size)
#define MAX_NUM_GPIO_PACKETS 4

// Max number of input and output cv gates that this protocol supports
#define MAX_NUM_CV_IN_GATES 16
#define MAX_NUM_CV_OUT_GATES 16

// Hardcoded size definitions
#define AUDIO_CONTROL_PACKET_SIZE 152
#define AUDIO_CONTROL_PACKET_SIZE_WORDS 38

/**
 * Command codes (MSB)
 */
typedef enum
{
    AUDIO_CMD_NULL = 0,
    AUDIO_CMD_MUTE = 100,
    AUDIO_CMD_UNMUTE = 101,
    AUDIO_CMD_CEASE = 102,
    GPIO_PACKET = 179,
    MIDI_PACKET = 186
} AudioControlCommands;

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
    uint8_t     payload[AUDIO_CONTROL_PACKET_PAYLOAD_SIZE];
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
} AudioControlPacket;

// statically verify the hardcoded size definitions

COMPILER_VERIFY(sizeof(AudioControlPacket) == AUDIO_CONTROL_PACKET_SIZE);
COMPILER_VERIFY(sizeof(AudioControlPacket)/4 == AUDIO_CONTROL_PACKET_SIZE_WORDS);
COMPILER_VERIFY(MAX_NUM_GPIO_PACKETS == AUDIO_CONTROL_PACKET_PAYLOAD_SIZE/GPIO_PACKET_SIZE);


#endif /* AUDIO_PROTOCOL_H_ */