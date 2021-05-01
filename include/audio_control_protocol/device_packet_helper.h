// SPDX-License-Identifier: GPL-2.0
/*
 * @copyright 2017-2029 Modern Ancient Instruments Networked AB, dba Elk,
 * Stockholm
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
 * @brief Create a packet with a command to enable input gain
 *
 * @param pkt The device control packet
 */
inline void prepare_enable_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_ENABLE_INPUT_GAIN;
}

/**
 * @brief Create a packet with a command to disable input gain
 *
 * @param pkt The device control packet
 */
inline void prepare_disable_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	create_default_device_ctrl_pkt(pkt);
	pkt->device_cmd = DEVICE_DISABLE_INPUT_GAIN;
}

/**
 * @brief Check if packet has a command to enable input gain
 *
 * @param pkt The device control packet
 * @return 1 if the packet has command to enable input gain, 0 if not
 */
inline int check_for_enable_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	if (pkt->device_cmd == DEVICE_ENABLE_INPUT_GAIN) {
		return 1;
	}

	return 0;
}

/**
 * @brief Check if packet has a command to disable input gain
 *
 * @param pkt The device control packet
 * @return 1 if the packet has command to disable input gain, 0 if not
 */
inline int check_for_disable_input_gain_cmd_pkt(struct device_ctrl_pkt* pkt)
{
	if (pkt->device_cmd == DEVICE_DISABLE_INPUT_GAIN) {
		return 1;
	}

	return 0;
}

#ifdef __cplusplus
} // namespace device_ctrl
#endif

#endif // DEVICE_PACKET_HELPER_H_
