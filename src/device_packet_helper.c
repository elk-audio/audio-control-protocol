/**
 * Copyright (C) 2018, Mind Music Labs AB, Stockholm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "device_control_protocol.h"
#include "device_packet_helper.h"

static const struct device_control_packet _DEFAULT_DEVICE_CONTROL_PACKET = {
	.magic_start = {'x', 'i'},
	.device_cmd = 0,
	.payload = {0},
	.magic_stop = 'd'
};

void inline create_default_device_control_packet(struct
						device_control_packet* packet)
{
	memcpy(packet, &_DEFAULT_DEVICE_CONTROL_PACKET,
				DEVICE_CONTROL_PACKET_SIZE);
}

int inline check_device_packet_for_magic_words(const struct
						device_control_packet* packet)
{
	if (packet->magic_start[0] != 'x' ||
		packet->magic_start[1] != 'i' ||
		packet->magic_stop != 'd') {
		return 0;
	}

	return 1;
}

uint8_t inline get_device_packet_cmd(const struct device_control_packet* packet)
{
	return packet->device_cmd;
}

void inline prepare_version_check_query_packet(struct
						device_control_packet* packet)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
}

void inline prepare_version_check_reply_packet(struct
						device_control_packet* packet,
						uint8_t major_vers,
						uint8_t minor_vers)
{
	uint8_t* version_data = (uint8_t*)packet->payload;
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_FIRMWARE_VERSION_CHECK;
	version_data[0] = major_vers;
	version_data[1] = minor_vers;
}

int inline check_if_version_matches(struct device_control_packet* packet,
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

void inline prepare_start_cmd_packet(struct device_control_packet* packet,
					int buffer_size)
{
	int* buffer_size_payload = (int*)packet->payload;
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_START;
	*buffer_size_payload = buffer_size;
}

int inline get_buffer_size_info(struct device_control_packet* packet)
{
	int* buffer_size_info = (int*)packet->payload;
	return *buffer_size_info;
}

void inline prepare_stop_cmd_packet(struct device_control_packet* packet)
{
	create_default_device_control_packet(packet);
	packet->device_cmd = DEVICE_STOP;
}