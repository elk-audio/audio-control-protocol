/*
 * Copyright 2018-2021 Modern Ancient Instruments Networked AB, dba Elk
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
 * @copyright 2018-2021 Modern Ancient Instruments Networked AB, dba Elk, Stockholm
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
	DEVICE_CHANGE_INPUT_GAIN = 124,
	DEVICE_CHANGE_HP_VOL = 125,
	DEVICE_SET_RGB_LED_VAL = 126,
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
 * @brief Represents the value that can be written to an RGB led.
 */
struct device_rgb_led_val {
	uint8_t brightness;
	uint8_t r_val;
	uint8_t g_val;
	uint8_t b_val;
};

/**
 * @brief Represents info sent along with a DEVICE_CHANGE_INPUT_GAIN command
 * @param gain_val The value of the gain that will be copied to the codec gain register
 * @param jack_id The jack id for which the gain has to be applied
 */
struct device_input_gain_data {
	uint32_t gain_val;
	uint32_t jack_id;
};


/**
 * @brief Represents info sent along with a DEVICE_SET_RGB_LED_VAL command.
 * @param rgb_led_id the led id to control
 * @param rgb_led_val the value to set for the led
 */
struct device_rgb_led_data {
	uint32_t rgb_led_id;
	struct device_rgb_led_val rgb_led_val;
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
	struct device_input_gain_data input_gain_data;
	uint32_t hp_vol_data;
	struct device_rgb_led_data rgb_led_data;
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
