/**
 * Copyright (C) 2018, Mind Music Labs AB, Stockholm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef DEVICE_PROTOCOL_H_
#define DEVICE_PROTOCOL_H_

#define DEVICE_PACKET_PAYLOAD_SIZE 8

#ifndef __KERNEL__
#include <stdint.h>
#endif

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
	//@ magic start chars 'x', 'i'
	uint8_t magic_start[2];
	//@ command msb & lsb
	uint8_t device_cmd;
	//@ command payload
	uint8_t payload[DEVICE_PACKET_PAYLOAD_SIZE];
	//@ magic stop char 'd'
	uint8_t magic_stop;
};

#define DEVICE_CONTROL_PACKET_SIZE sizeof(device_control_packet)

#endif