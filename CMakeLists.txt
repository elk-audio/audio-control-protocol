cmake_minimum_required(VERSION 3.4)
project(audio_control_protocol)

set(SOURCE_FILES src/audio_packet_helper.c
                 src/device_packet_helper.c)

add_library(audio_control_protocol STATIC ${SOURCE_FILES})
target_include_directories(audio_control_protocol PUBLIC ${PROJECT_SOURCE_DIR}/include)