/*
 * Copyright 2022 Modern Ancient Instruments Networked AB, dba Elk
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
 * @brief Protocol definition for audio channel status info sharing. Channel
 *          status information is meant to be updated by the microcontroller
 *          and is read only for the host processor.
 * @copyright 2022 Modern Ancient Instruments Networked AB, dba Elk, Stockholm
 */
#ifndef AUDIO_CH_STATUS_H_
#define AUDIO_CH_STATUS_H_

#include "audio_protocol_common.h"

#ifdef __cplusplus
namespace audio_ctrl {
#endif

// The status of each audio channel
typedef struct
{
    uint8_t flags;  // flags
    uint8_t rfu[3]; // reserved for future use
} AudioChStatus;

// Channel unmute flag
#define AUDIO_CH_STATUS_FLAGS_UNMUTE    0x01u

// Get the 16 byte aligned size of the channel status array in words
#define AUDIO_CH_STATUS_ARRAY_SIZE_IN_WORDS(_num_channels)   (4 * ((_num_channels + 3) / 4))

// Get the 16 byte aligned size of the channel status array in bytes
#define AUDIO_CH_STATUS_ARRAY_SIZE_IN_BYTES(_num_channels) \
    (4 * AUDIO_CH_STATUS_ARRAY_SIZE_IN_WORDS(_num_channels))

// statically verify the hardcoded size definitions
COMPILER_VERIFY(sizeof(AudioChStatus) == 4);

#ifdef __cplusplus
} // namespace audio_ctrl
#endif

#endif // AUDIO_CH_STATUS_H_
