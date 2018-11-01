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

#ifdef __cplusplus__
extern "C" {
#endif

/**
 * @brief Creates a default device control packet.
 *
 * @param packet the device control packet
 */
void create_default_device_control_packet(struct device_control_packet* packet);

/**
 * @brief checks for the precence of magic words in the packet
 *
 * @param packet the device control packet
 * @return 1 if packet contains magic words, 0 if not
 */
int check_device_packet_for_magic_words(const struct device_control_packet* packet);

/**
 * @brief Gets the command in a device packet.
 *
 * @param packet the device control packet
 * @return The device packet command.
 */
uint8_t get_device_packet_cmd(const struct device_control_packet* packet);

/**
 * @brief Prepares a version check query packet
 *
 * @param packet The device control packet
 */
void prepare_version_check_query_packet(struct device_control_packet* packet);

/**
 * @brief Prepares a version check reply packet with the version info
 *
 * @param packet The device control packet
 * @param major_vers The major vers
 * @param minor_vers The minor vers
 */
void prepare_version_check_reply_packet(struct device_control_packet* packet,
					uint8_t major_vers,
					uint8_t minor_vers);

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
int check_if_version_matches(struct device_control_packet* packet,
				uint8_t expected_major_vers,
				uint8_t expected_minor_vers);

/**
 * @brief prepares a start cmd packet
 *
 * @param packet the device control packet
 * @param the buffers size which will be inserted into the packets payload
 */
void prepare_start_cmd_packet(struct device_control_packet* packet,
				int buffer_size);

/**
 * @brief prepares a stop cmd packet
 *
 * @param packet the device control packet
 */
void prepare_stop_cmd_packet(struct device_control_packet* packet);

#ifdef __cplusplus__
} // extern C
#endif

#endif