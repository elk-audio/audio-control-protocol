// SPDX-License-Identifier: GPL-2.0
/*
 * @copyright 2017-2019 Modern Ancient Instruments Networked AB, dba Elk,
 * Stockholm
 */

/**
 * @brief Helper functions that can be used by both the host machine and
 *        secondary microcontrollers to process and manipulate device control
 *        packets
 */
#ifndef DEVICE_PACKET_HELPER_H_
#define DEVICE_PACKET_HELPER_H_

#include "device_control_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Clears the packet memory location. This is needed as memset discards
 *        volatile qualifier and in turn causes problems.
 *
 * @param packet the device control packet
 */
inline void clear_device_control_packet(struct device_control_packet* const
					packet)
{
	uint32_t* ptr = (uint32_t*) packet;
	int i;

	for(i = 0; i < DEVICE_CONTROL_PACKET_SIZE_WORDS; i++) {
		ptr[i] = 0;
	}
}

/**
 * @brief Creates a default device control packet.
 *
 * @param packet the device control packet
 */
inline void create_default_device_control_packet(struct device_control_packet*
						const packet)
{
	clear_device_control_packet(packet);
	packet->magic_start[0] = 'x';
	packet->magic_start[1] = 'i';
	packet->magic_stop = 'd';
}

/**
 * @brief checks for the presence of magic words in the packet
 *
 * @param packet the device control packet
 * @return 1 if packet contains magic words, 0 if not
 */
inline int check_device_packet_for_magic_words(const struct
					device_control_packet* const packet)
{
	if (packet->magic_start[0] != 'x' ||
		packet->magic_start[1] != 'i' ||
		packet->magic_stop != 'd') {
		return 0;
	}

	return 1;
}

/**
 * @brief Check if packet has version check cmd
 * 
 * @param packet The device command packet
 * @return int 1 if packet has version check cmd, 0 if not
 */
inline int check_for_version_check_cmd(const struct  device_control_packet*
					const packet)
{
	if (packet->device_cmd == DEVICE_FIRMWARE_VERSION_CHECK) {
		return 1;
	}

	return 0;
}

/**
 * @brief Prepares a version check query packet
 *
 * @param packet The device control packet
 */
inline void prepare_version_check_query_packet(struct device_control_packet*
						const packet)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
}

/**
 * @brief Prepares a version check reply packet with the version info
 *
 * @param packet The device control packet
 * @param major_vers The major vers
 * @param minor_vers The minor vers
 */
inline void prepare_version_check_reply_packet(struct device_control_packet*
						const packet,
						uint8_t major_vers,
						uint8_t minor_vers)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
	packet->payload[0] = major_vers;
	packet->payload[1] = minor_vers;
}

/**
 * @brief Parses the device control packet's payload to retrieve the version
 *        data (assuming that the packet containing version check cmd and data)
 *        and compares it with the expected version
 *
 * @param packet The device control packet
 * @param expected_major_vers The expected major version
 * @param expected_minor_vers The expected minor version
 *
 * @return 1 if version matches, 0 if not
 */
inline int check_if_version_matches(const struct device_control_packet*
					const packet,
					uint8_t expected_major_vers,
					uint8_t expected_minor_vers)
{
	if (packet->payload[0] != expected_major_vers ||
		packet->payload[1] != expected_minor_vers) {
		return 0;
	}

	return 1;
}

/**
 * @brief Check for start cmd in the device packet
 *
 * @param packet The device control packet
 * @return int Buffer size if packet has the start cmd, 0 if not
 */
inline int check_for_start_cmd(const struct device_control_packet* const packet)
{
	int* buffer_size = (int*) packet->payload;

	if (packet->device_cmd == DEVICE_START) {
		return *buffer_size;
	}

	return 0;
}

/**
 * @brief prepares a start cmd packet
 *
 * @param packet the device control packet
 * @param the buffers size which will be inserted into the packets payload
 */
inline void prepare_start_cmd_packet(struct device_control_packet* const packet,
					int buffer_size)
{
	int* payload = (int*) packet->payload;

	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_START;
	*payload = buffer_size;
}

inline int check_for_stop_cmd(const struct device_control_packet* const packet)
{
	if (packet->device_cmd == DEVICE_STOP) {
		return 1;
	}

	return 0;
}

/**
 * @brief prepares a stop cmd packet
 *
 * @param packet the device control packet
 */
inline void prepare_stop_cmd_packet(struct device_control_packet* packet)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_STOP;
}

#ifdef __cplusplus
} // extern C
#endif

#endif // DEVICE_PACKET_HELPER_H_
