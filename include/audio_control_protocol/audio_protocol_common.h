// SPDX-License-Identifier: GPL-2.0
/*
 * @copyright 2017-2019 Modern Ancient Instruments Networked AB, dba Elk,
 * Stockholm
 */

/**
 * @brief Common includes and macros for the protocol
 */

#ifndef AUDIO_PROTOCOL_COMMON_H_
#define AUDIO_PROTOCOL_COMMON_H_

#define AUDIO_PROTOCOL_VERSION_MAJ 0
#define AUDIO_PROTOCOL_VERSION_MIN 2
#define AUDIO_PROTOCOL_VERSION_REV 0

// static assert implementation for xmos platform
#ifdef __XC__
#define GLUE(a,b) __GLUE(a,b)
#define __GLUE(a,b) a ## b
#define CVERIFY(expr, msg) typedef char GLUE (compiler_verify_, msg) [(expr) ? (+1) : (-1)]
#define COMPILER_VERIFY(exp) CVERIFY (exp, __LINE__)

// generic c++ static assert for platforms using C++
#elif defined (__cplusplus)
#include <stdint.h>
#include <assert.h>
#define COMPILER_VERIFY(exp) static_assert(exp)

// empty macro otherwise
#else
#define COMPILER_VERIFY(exp)

#endif

#endif // AUDIO_PROTOCOL_COMMON_H_