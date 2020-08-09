// SPDX-License-Identifier: GPL-2.0
/*
 * @copyright 2017-2019 Modern Ancient Instruments Networked AB, dba Elk,
 * Stockholm
 */

/**
 * @brief Contains the protocol that can be used by a driver in a host machine
 *        to comunicate and control secondary microcontrollers.
 */
#ifndef DEVICE_PROTOCOL_H_
#define DEVICE_PROTOCOL_H_

#include "audio_protocol_common.h"

// Denotes the payload size in bytes that this protocol can carry
#define DEVICE_PACKET_PAYLOAD_SIZE 11

/**
 * enumeration of system commands
 */
enum device_commands {
	DEVICE_CMD_NULL = 0,
	DEVICE_FIRMWARE_VERSION_CHECK = 191,
	DEVICE_START = 123,
	DEVICE_STOP = 234
};

/**
 * @brief device control packet structure
 */
struct device_control_packet {
	// magic start chars 'x', 'i'
	uint8_t magic_start[2];

	// command msb
	uint8_t device_cmd;

	// reserved data for padding
	uint8_t reserved;

	// command payload
	uint8_t payload[DEVICE_PACKET_PAYLOAD_SIZE];

	// magic stop char 'd'
	uint8_t magic_stop;
};

// Hardcoded size definitions to help optimization of loops.
#define DEVICE_CONTROL_PACKET_SIZE 16
#define DEVICE_CONTROL_PACKET_SIZE_WORDS 4

COMPILER_VERIFY(sizeof(struct device_control_packet) ==
	DEVICE_CONTROL_PACKET_SIZE);
COMPILER_VERIFY(sizeof(struct device_control_packet)/4 ==
	DEVICE_CONTROL_PACKET_SIZE_WORDS);

#endif // DEVICE_PROTOCOL_H_