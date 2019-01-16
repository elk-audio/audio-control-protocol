/**
 * Copyright (C) 2018, Mind Music Labs AB, Stockholm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef DEVICE_PACKET_HELPER_H_
#define DEVICE_PACKET_HELPER_H_

#include "device_control_protocol.h"

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a default device control packet.
 *
 * @param packet the device control packet
 */
inline void create_default_device_control_packet(struct device_control_packet*
							packet)
{
	packet->magic_start[0] = 'x';
	packet->magic_start[1] = 'i';
	packet->magic_stop = 'd';
	memset(packet->payload, 0, DEVICE_PACKET_PAYLOAD_SIZE);
}
/**
 * @brief checks for the precence of magic words in the packet
 *
 * @param packet the device control packet
 * @return 1 if packet contains magic words, 0 if not
 */
inline int check_device_packet_for_magic_words(const struct
						device_control_packet* packet)
{
	if (packet->magic_start[0] != 'x' ||
		packet->magic_start[1] != 'i' ||
		packet->magic_stop != 'd') {
		return 0;
	}

	return 1;
}

/**
 * @brief Gets the command in a device packet.
 *
 * @param packet the device control packet
 * @return The device packet command.
 */
inline uint8_t get_device_packet_cmd(const struct device_control_packet* packet)
{
	return packet->device_cmd;
}
/**
 * @brief Prepares a version check query packet
 *
 * @param packet The device control packet
 */
inline void prepare_version_check_query_packet(struct device_control_packet*
						packet)
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
inline void prepare_version_check_reply_packet(struct device_control_packet* packet,
					uint8_t major_vers,
					uint8_t minor_vers)
{
	uint8_t* version_data = (uint8_t*)packet->payload;
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
	version_data[0] = major_vers;
	version_data[1] = minor_vers;
}

/**
 * @brief Parses the device control packet's payload to retrieve the version
 *        data (assuming that the packet containing versiob check cmd and data)
 *        and compares it with the expected version
 *
 * @param packet The device control packet
 * @param expected_major_vers The expected major version
 * @param expected_minor_vers The expected minor version
 *
 * @return 1 if version matches, 0 if not
 */
inline int check_if_version_matches(struct device_control_packet* packet,
				uint8_t expected_major_vers,
				uint8_t expected_minor_vers)
{
	uint8_t* version_data = packet->payload;
	if (version_data[0] != expected_major_vers ||
		version_data[1] != expected_minor_vers) {
		return 0;
	}

	return 1;
}

/**
 * @brief prepares a start cmd packet
 *
 * @param packet the device control packet
 * @param the buffers size which will be inserted into the packets payload
 */
inline void prepare_start_cmd_packet(struct device_control_packet* packet,
				int buffer_size)
{
	int* buffer_size_payload = (int*)packet->payload;
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_START;
	*buffer_size_payload = buffer_size;
}

/**
 * @brief Get the buffer size info from the payload
 *
 * @param packet the device control packet
 * @return int the buffer size
 */
inline int get_buffer_size_info(struct device_control_packet* packet)
{
	int* buffer_size_info = (int*)packet->payload;
	return *buffer_size_info;
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

#endif