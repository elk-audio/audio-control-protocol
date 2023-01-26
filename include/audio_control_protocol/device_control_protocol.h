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
#define DEVICE_CTRL_PKT_PAYLOAD_SIZE 120

// Hat name size including null terminator
#define DEVICE_CTRL_PKT_HAT_NAME_SIZE 32

// Audio channel name size including null terminator
#define DEVICE_CTRL_PKT_AUDIO_CHANNEL_NAME_SIZE 32

/**
 * @brief Enumeration of system commands
 */
enum device_commands {
	DEVICE_CMD_NULL = 0,
	DEVICE_PING = 1,
	DEVICE_FIRMWARE_VERSION_CHECK = 191,
	DEVICE_SYSTEM_INFO = 192,
	DEVICE_AUDIO_CHANNEL_INFO = 193,
	DEVICE_START = 123,
	DEVICE_CHANGE_INPUT_GAIN = 124,
	DEVICE_CHANGE_HP_VOL = 125,
	DEVICE_SET_RGB_LED_VAL = 126,
	DEVICE_STOP = 234,
	DEVICE_RAW_DATA = 254,
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
 * @brief Represents the system informations data.
 */
struct system_info_data {
	uint8_t	 hat_name[DEVICE_CTRL_PKT_HAT_NAME_SIZE];	// Hat name including the null terminator
	uint32_t flags;						// Bit mask of DEVICE_CTRL_SYSTEM_INFO_FLAGS_xxx flags
	uint32_t	sampling_rate;				// Audio sampling rate in Hertz
	uint8_t	 num_audio_inputs;				// Number of audio input channels
	uint8_t	 num_audio_outputs;				// Number of audio output channels
	uint8_t	 num_midi_inputs;				// Number of MIDI input ports
	uint8_t	 num_midi_outputs;				// Number of MIDI output ports
};

// System info flags definition
#define DEVICE_CTRL_SYSTEM_INFO_FLAGS_HAS_MICROCONTROLLER_USB	0x00000001u

/**
 * @brief Represents the audio channel direction.
 */
enum audio_channel_direction {
	INPUT_DIRECTION = 0,
	OUTPUT_DIRECTION = 1,
};

/**
 * @brief Represents the audio sample format each channel has.
 */
enum audio_sample_format {
	INT24_LJ = 1,	// 24 bit samples left justified. Format : 0xXXXXXX00
	INT24_I2S,	// 24 bit samples I2S format. The first bit is always 0
	INT24_RJ,	// 24 bit samples right justified. Format : 0x00XXXXXX
	INT24_32RJ,	// 24 bit samples converted into 32 bit samples
	INT32,		// 32 bit samples
	BINARY,		// represents binary data. No ops should be done on such chans
};

/**
 * @brief Represents the information available for each audio channel.
 */
struct audio_channel_info_req {
	uint32_t buffer_size_in_frames;			// The audio buffer size in frames
	uint8_t sw_ch_id;						// The software channel ID
	uint8_t direction;						// The audio channel direction as of audio_channel_direction enum
};

/**
 * @brief Represents the information available for each audio channel.
 */
struct audio_channel_info_data {
	uint8_t sw_ch_id;						// The software channel ID or DEVICE_CTRL_AUDIO_CHANNEL_NOT_VALID
	uint8_t hw_ch_id;						// The hardware channel ID or DEVICE_CTRL_AUDIO_CHANNEL_NOT_VALID
	uint8_t direction;						// The audio channel direction as of audio_channel_direction enum
	uint8_t sample_format;						// The sample format as of audio_sample_format enum
	uint8_t channel_name[DEVICE_CTRL_PKT_AUDIO_CHANNEL_NAME_SIZE];  // The channe name must be a valid null terminated string
	uint32_t start_offset_in_words;					// Audio channel data start offset in words
	uint32_t stride_in_words;					// Audio channel data stride in words
};

#define DEVICE_CTRL_AUDIO_CHANNEL_NOT_VALID 255

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
	uint32_t ping_code;
	struct system_info_data system_info_data;
	struct audio_channel_info_req audio_channel_info_req;
	struct audio_channel_info_data audio_channel_info_data;
	int buffer_size;
	struct device_version_data version_data;
	struct device_input_gain_data input_gain_data;
	uint32_t hp_vol_data;
	struct device_rgb_led_data rgb_led_data;
};

/**
 * @brief Device control packet structure
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
#define DEVICE_CTRL_PKT_SIZE 128
#define DEVICE_CTRL_PKT_SIZE_WORDS 32

COMPILER_VERIFY(sizeof(struct device_ctrl_pkt) == DEVICE_CTRL_PKT_SIZE);
COMPILER_VERIFY(sizeof(struct device_ctrl_pkt)/4 == DEVICE_CTRL_PKT_SIZE_WORDS);
COMPILER_VERIFY(sizeof(union device_pkt_payload) == DEVICE_CTRL_PKT_PAYLOAD_SIZE);
COMPILER_VERIFY(sizeof(struct system_info_data)%4 == 0);
COMPILER_VERIFY(sizeof(struct audio_channel_info_data)%4 == 0);

#ifdef __cplusplus
} // namespace device_ctrl
#endif

#endif // DEVICE_PROTOCOL_H_
