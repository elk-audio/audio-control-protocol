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
 * @brief Common includes and macros for the protocol
 */

#ifndef AUDIO_PROTOCOL_COMMON_H_
#define AUDIO_PROTOCOL_COMMON_H_

#define AUDIO_PROTOCOL_VERSION_MAJ 0
#define AUDIO_PROTOCOL_VERSION_MIN 4
#define AUDIO_PROTOCOL_VERSION_REV 1

// static assert implementation for xmos platform
#ifdef __XC__
#define GLUE(a,b) __GLUE(a,b)
#define __GLUE(a,b) a ## b
#define CVERIFY(expr, msg) typedef char GLUE (compiler_verify_, msg) [(expr) ? (+1) : (-1)]
#define COMPILER_VERIFY(exp) CVERIFY (exp, __LINE__)

#include <stdint.h>

// generic c++ static assert for platforms using C++
#elif defined (__cplusplus)
#include <cstdint>
#include <assert.h>
#define COMPILER_VERIFY(exp) static_assert(exp)

// empty macro otherwise
#else
#define COMPILER_VERIFY(exp)

#endif

#endif // AUDIO_PROTOCOL_COMMON_H_
