cmake_minimum_required(VERSION 3.8)
project(audio_control_protocol)

set(COMPILATION_UNITS src/audio_packet_helper.c
                      src/device_packet_helper.c)

add_library(audio_control_protocol STATIC ${COMPILATION_UNITS})
target_include_directories(audio_control_protocol PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(audio_control_protocol PUBLIC c_std_99)
target_compile_options(audio_control_protocol PRIVATE -Wall -Wextra)