// SPDX-License-Identifier: GPL-2.0
/*
 * @copyright 2017-2020 Modern Ancient Instruments Networked AB, dba Elk,
 * Stockholm
 */

/**
 * @brief Contains the protocol that can be used by a driver in a host machine
 *        to comunicate and control secondary microcontrollers.
 */
#ifndef DEVICE_PROTOCOL_H_
#define DEVICE_PROTOCOL_H_

#include "audio_protocol_common.h"

#ifdef __cplusplus
namespace device_ctrl {
#endif

// Denotes the payload size in bytes that this protocol can carry
#define DEVICE_CTRL_PKT_PAYLOAD_SIZE 8

/**
 * @brief enumeration of system commands
 */
enum device_commands {
	DEVICE_CMD_NULL = 0,
	DEVICE_FIRMWARE_VERSION_CHECK = 191,
	DEVICE_START = 123,
	DEVICE_ENABLE_INPUT_GAIN = 124,
	DEVICE_DISABLE_INPUT_GAIN = 125,
	DEVICE_STOP = 234
};

/**
 * @brief Represents the firmware version information
 */
struct device_version_data {
	uint8_t major_vers;
	uint8_t minor_vers;
	uint8_t board_vers;
};

/**
 * @brief Union representing the various data that can constitute an device
 *        control packet's payload. The total size should be equal to
 *        DEVICE_CTRL_PKT_PAYLOAD_SIZE
 */
union device_pkt_payload {
	uint8_t raw_data[DEVICE_CTRL_PKT_PAYLOAD_SIZE];
	int buffer_size;
	struct device_version_data version_data;
};

/**
 * @brief device control packet structure
 */
struct device_ctrl_pkt {
	// magic start chars 'x', 'i'
	uint8_t magic_start[2];

	// command msb
	uint8_t device_cmd;

	// reserved data for padding
	uint8_t device_subcmd;

	// command payload
	union device_pkt_payload payload;

	// reserved data for padding
	uint8_t reserved[3];

	// magic stop char 'd'
	uint8_t magic_stop;
};

// Hardcoded size definitions to help optimization of loops.
#define DEVICE_CTRL_PKT_SIZE 16
#define DEVICE_CTRL_PKT_SIZE_WORDS 4

COMPILER_VERIFY(sizeof(struct device_ctrl_pkt) == DEVICE_CTRL_PKT_SIZE);
COMPILER_VERIFY(sizeof(struct device_ctrl_pkt)/4 == DEVICE_CTRL_PKT_SIZE_WORDS);
COMPILER_VERIFY(sizeof(union device_pkt_payload) == DEVICE_CTRL_PKT_PAYLOAD_SIZE);

#ifdef __cplusplus
} // namespace device_ctrl
#endif

#endif // DEVICE_PROTOCOL_H_