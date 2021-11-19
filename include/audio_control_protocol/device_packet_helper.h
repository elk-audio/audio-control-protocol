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
 * @brief Helper functions that can be used by both the host machine and
 *        secondary microcontrollers to parse or generate device control
 *        packets
 */
#ifndef DEVICE_PACKET_HELPER_H_
#define DEVICE_PACKET_HELPER_H_

#include "device_control_protocol.h"

#ifdef __cplusplus
namespace device_ctrl {
#endif

/**
 * @brief Clears the packet.
 *
 * @param pkt the device control packet
 */
#ifdef __XC__
#pragma loop unroll
#pragma unsafe arrays
#endif
inline void clear_device_ctrl_pkt(struct device_ctrl_pkt* const pkt)
{
	uint32_t* pkt_data = (uint32_t*) pkt;
	int i;

	for (i = 0; i < DEVICE_CTRL_PKT_SIZE_WORDS; i++) {
		pkt_data[i] = 0;
	}
}

/**
 * @brief Creates a default device control packet.
 *
 * @param pkt the device control packet
 */
inline void create_default_device_ctrl_pkt(struct device_ctrl_pkt*
						const pkt)
{
	clear_device_ctrl_pkt(pkt);
	pkt->magic_start[0] = 'x';
	pkt->magic_start[1] = 'i';
	pkt->magic_stop = 'd';
	pkt->device_cmd = DEVICE_CMD_NULL;
}

/**
 * @brief checks for the presence of magic words in the packet
 *
 * @param pkt the device control packet
 * @return 1 if packet contains magic words, 0 if not
 */
inline int check_device_pkt_for_magic_words(const struct
					device_ctrl_pkt* const pkt)
{
	if (pkt->magic_start[0] != 'x' ||
		pkt->magic_start[1] != 'i' ||
		pkt->magic_stop != 'd') {
		return 0;
	}

	return 1;
}

/**
 * @brief Check if a device packet has a null command
 *
 * @param pkt The device control packet
 * @return int 1 if true, 0 if false
 */
inline int check_device_pkt_for_null_cmd(const struct
					device_ctrl_pkt* const pkt)
{
	if (pkt->device_cmd != DEVICE_CMD_NULL) {
		return 0;
	}

	return 1;
}

/**
 * @brief Check if packet has version check cmd
 *
 * @param pkt The device command packet
 * @return int 1 if packet has version check cmd, 0 if not
 */
inline int check_for_version_check_cmd(const struct device_ctrl_pkt*
					const pkt)
{
	if (pkt->device_cmd == DEVICE_FIRMWARE_VERSION_CHECK) {
		return 1;
	}

	return 0;
}

/**
 * @brief Prepares a version check query packet
 *
 * @param pkt The device control packet
 */
inline void prepare_version_check_query_pkt(struct device_ctrl_pkt* const pkt)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
}

/**
 * @brief Prepares a version check reply packet with the version info
 *
 * @param pkt The device control packet
 * @param major_vers The major vers
 * @param minor_vers The minor vers
 */
inline void prepare_version_check_reply_pkt(struct device_ctrl_pkt* const pkt,
						uint8_t major_vers,
						uint8_t minor_vers,
						uint8_t board_vers)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
	pkt->payload.version_data.major_vers = major_vers;
	pkt->payload.version_data.minor_vers = minor_vers;
	pkt->payload.version_data.board_vers = board_vers;
}

/**
 * @brief Parses the device control packet's payload to retrieve the version
 *        data (assuming that the packet containing version check cmd and data)
 *        and compares it with the expected version
 *
 * @param pkt The device control packet
 * @param expected_major_vers The expected major version
 * @param expected_minor_vers The expected minor version
 *
 * @return 1 if version matches, 0 if not
 */
inline int check_if_fw_vers_matches(const struct device_ctrl_pkt* const pkt,
					uint8_t expected_major_vers,
					uint8_t expected_minor_vers)
{
	if (pkt->device_cmd != DEVICE_FIRMWARE_VERSION_CHECK ||
		pkt->payload.version_data.major_vers != expected_major_vers ||
		pkt->payload.version_data.minor_vers != expected_minor_vers) {
		return 0;
	}

	return 1;
}

/**
 * @brief Get the board version from the packet,(assuming that the packet
 *        containing version check cmd and data).
 *
 * @param pkt the device control packet containing the reply to a version check
 *            command
 * @return int The board version
 */
inline uint8_t get_board_vers(const struct device_ctrl_pkt* const pkt)
{
	return pkt->payload.version_data.board_vers;
}

/**
 * @brief Check for start cmd in the device packet
 *
 * @param pkt The device control packet
 * @return int Buffer size if packet has the start cmd, 0 if not
 */
inline int check_for_start_cmd(const struct device_ctrl_pkt* const pkt)
{
	if (pkt->device_cmd == DEVICE_START) {
		return pkt->payload.buffer_size;
	}

	return 0;
}

/**
 * @brief prepares a start cmd packet
 *
 * @param pkt the device control packet
 * @param the buffers size which will be inserted into the packets payload
 */
inline void prepare_start_cmd_pkt(struct device_ctrl_pkt* const pkt,
					int buffer_size)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_START;
	pkt->payload.buffer_size = buffer_size;
}

/**
 * @brief Check for a stop command in the packet
 *
 * @param pkt The device control packet
 * @return int 1 if there is a stop command, 0 if not
 */
inline int check_for_stop_cmd(const struct device_ctrl_pkt* const pkt)
{
	if (pkt->device_cmd == DEVICE_STOP) {
		return 1;
	}

	return 0;
}

/**
 * @brief prepares a stop cmd packet
 *
 * @param pkt the device control packet
 */
inline void prepare_stop_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_STOP;
}

/**
 * @brief Create a packet with a command to set input gain for a jack
 *
 * @param pkt The device control packet
 * @param gain_val The value to be written to the codec's gain register
 * @param jack_id the jack for which the input gain has to be changed.
 */
inline void prepare_change_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt,
					uint32_t gain_val,
					uint32_t jack_id)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_CHANGE_INPUT_GAIN;
	pkt->payload.input_gain_data.gain_val = gain_val;
	pkt->payload.input_gain_data.jack_id = jack_id;
}


/**
 * @brief check if packet has a change input gain command
 *
 * @param pkt The device control packet
 * @return 1 if true, 0 if not
 */
inline int check_for_change_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	if (pkt->device_cmd == DEVICE_CHANGE_INPUT_GAIN) {
		return 1;
	}

	return 0;
}

inline struct device_input_gain_data* get_change_input_gain_data
						(struct device_ctrl_pkt* pkt)
{
	return &pkt->payload.input_gain_data;
}

/**
 * @brief Create a packet with a command to set output vol
 *
 * @param pkt The device control packet
 * @param vol_val The value to be written to the codec's hp vol register
 */
inline void prepare_change_hp_vol_cmd_pkt(struct device_ctrl_pkt* pkt,
					uint32_t vol_val)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_CHANGE_HP_VOL;
	pkt->payload.hp_vol_data = vol_val;
}


/**
 * @brief check if packet has a change hp vol command
 *
 * @param pkt The device control packet
 * @return 1 if true, 0 if not
 */
inline int check_for_change_hp_vol_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	if (pkt->device_cmd == DEVICE_CHANGE_HP_VOL) {
		return 1;
	}

	return 0;
}

/**
 * @brief Get the change hp vol data
 *
 * @param pkt The device control packet
 * @return uint32_t The hp vol value
 */
inline uint32_t get_change_hp_vol_data(struct device_ctrl_pkt* pkt)
{
	return pkt->payload.hp_vol_data;
}

inline void prepare_set_rgb_led_val_cmd(struct device_ctrl_pkt* pkt,
					uint32_t rgb_led_id,
					struct device_rgb_led_val* rgb_led_val)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_SET_RGB_LED_VAL;

	pkt->payload.rgb_led_data.rgb_led_id = rgb_led_id;

	pkt->payload.rgb_led_data.rgb_led_val.brightness =
							rgb_led_val->brightness;
	pkt->payload.rgb_led_data.rgb_led_val.r_val = rgb_led_val->r_val;
	pkt->payload.rgb_led_data.rgb_led_val.g_val = rgb_led_val->g_val;
	pkt->payload.rgb_led_data.rgb_led_val.b_val = rgb_led_val->b_val;
}

inline int check_for_rgb_led_val_cmd_pkt(struct device_ctrl_pkt* pkt) {
	if (pkt->device_cmd == DEVICE_SET_RGB_LED_VAL) {
		return 1;
	}

	return 0;
}

inline struct device_rgb_led_data* get_rgb_led_data(struct device_ctrl_pkt* pkt)
{
	return &pkt->payload.rgb_led_data;
}

#ifdef __cplusplus
} // namespace device_ctrl
#endif

#endif // DEVICE_PACKET_HELPER_H_
