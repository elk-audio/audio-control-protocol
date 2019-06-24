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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Clears the packet memory location. This is needed as memset discards 
 *        volatile qualifier and in turn causes problems.
 *
 * @param packet the device control packet
 */
static inline void clear_device_control_packet(volatile struct device_control_packet*
							packet)
{
	volatile uint8_t* ptr = (uint8_t*)packet;
	int i;

	for(i = 0; i < DEVICE_CONTROL_PACKET_SIZE; i++) {
		ptr[i] = 0;
	}
}

/**
 * @brief Creates a default device control packet.
 *
 * @param packet the device control packet
 */
static inline void create_default_device_control_packet(volatile struct device_control_packet*
							packet)
{
	int i;
	packet->magic_start[0] = 'x';
	packet->magic_start[1] = 'i';
	packet->magic_stop = 'd';
	for(i = 0; i < DEVICE_PACKET_PAYLOAD_SIZE; i++) {
		packet->payload[i] = 0x00;
	}
}
/**
 * @brief checks for the precence of magic words in the packet
 *
 * @param packet the device control packet
 * @return 1 if packet contains magic words, 0 if not
 */
static inline int check_device_packet_for_magic_words(volatile const struct
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
static inline uint8_t get_device_packet_cmd(volatile const struct device_control_packet* packet)
{
	return packet->device_cmd;
}
/**
 * @brief Prepares a version check query packet
 *
 * @param packet The device control packet
 */
static inline void prepare_version_check_query_packet(volatile struct device_control_packet*
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
static inline void prepare_version_check_reply_packet(volatile struct device_control_packet* packet,
					uint8_t major_vers,
					uint8_t minor_vers)
{
	volatile uint8_t* version_data = (uint8_t*)packet->payload;
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
static inline int check_if_version_matches(volatile struct device_control_packet* packet,
				uint8_t expected_major_vers,
				uint8_t expected_minor_vers)
{
	volatile uint8_t* version_data = packet->payload;
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
static void prepare_start_cmd_packet(volatile struct device_control_packet* packet,
				const int buffer_size)
{
	uint8_t* buffer_size_ptr = (uint8_t*)&buffer_size;
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_START;
	packet->payload[0] = buffer_size_ptr[0];
	packet->payload[1] = buffer_size_ptr[1];
	packet->payload[2] = buffer_size_ptr[2];
	packet->payload[3] = buffer_size_ptr[3];
}

/**
 * @brief Get the buffer size info from the payload
 *
 * @param packet the device control packet
 * @return int the buffer size
 */
static inline int get_buffer_size_info(volatile struct device_control_packet* packet)
{
	volatile int* buffer_size_info = (int*)packet->payload;
	return *buffer_size_info;
}

/**
 * @brief prepares a stop cmd packet
 *
 * @param packet the device control packet
 */
static inline void prepare_stop_cmd_packet(volatile struct device_control_packet* packet)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_STOP;
}

#ifdef __cplusplus
} // extern C
#endif

#endif
