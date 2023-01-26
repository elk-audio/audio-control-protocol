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
 * @brief Helper functions to parse or generate audio channel status data.
 * @copyright 2022 Modern Ancient Instruments Networked AB, dba Elk, Stockholm
 */
#ifndef AUDIO_CH_STATUS_HELPER_H_
#define AUDIO_CH_STATUS_HELPER_H_

#include "audio_control_protocol.h"
#include "audio_ch_status_protocol.h"

#ifdef __cplusplus
namespace audio_ctrl {
#endif

/**
 * @brief Initialize an audio channel status array
 *
 * @param base The base address of the audio channel status array
 * @param first_ch_idx The index of the first audio channel to be initialized
 * @param num_ch The number of audio channels starting from first_ch_idx that need to be initialized
 */
inline void init_audio_ch_status(AudioChStatus *base, int first_ch_idx, int num_ch)
{
    for (int i = 0; i < num_ch; i++)
    {
        base[first_ch_idx + i].flags = 0;       // muted
        base[first_ch_idx + i].rfu[0] = 0;
        base[first_ch_idx + i].rfu[1] = 0;
        base[first_ch_idx + i].rfu[2] = 0;
    }
}

/**
 * @brief Get the muting status of a specific audio channel
 *
 * @param base The base address of the audio channel status array
 * @param ch_idx The index of the audio channel
 * @return true if the channel is muted otherwise false
 */
inline bool get_audio_ch_mute(AudioChStatus *base, int ch_idx)
{
    return (base[ch_idx].flags & AUDIO_CH_STATUS_FLAGS_UNMUTE) ? false : true;
}

/**
 * @brief Set the audio channel muting state by setting the flags accordingly.
 *
 * @param base The base address of the audio channel status array
 * @param first_ch_idx The index of the first audio channel that need to be muted
 * @param num_ch The number of audio channels starting from first_ch_idx that need to be muted
 * @param muted The mute state to be set (true to enable muting)
 * @return true if any of the channel status flags is changed by function call otherwise false
 */
inline bool set_audio_ch_mute(AudioChStatus *base, int first_ch_idx, int num_ch, bool muted)
{
    bool changed = false;
    uint8_t old_flags;
    uint8_t new_flags;

    for (int i = 0; i < num_ch; i++)
    {
        old_flags = base[first_ch_idx + i].flags;
        new_flags = muted ? (old_flags & ~AUDIO_CH_STATUS_FLAGS_UNMUTE) :
                            (old_flags | AUDIO_CH_STATUS_FLAGS_UNMUTE);
        if (new_flags != old_flags)
        {
            base[first_ch_idx + i].flags = new_flags;
            changed = true;
        }
    }

    return changed;
}

#ifdef __cplusplus
} // namespace audio_ctrl
#endif

#endif // AUDIO_CH_STATUS_HELPER_H_
