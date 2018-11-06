#include <string.h>

#include "audio_control_protocol/audio_control_protocol.h"
#include "audio_control_protocol/audio_packet_helper.h"

// TODO : this cna be a problem if gpio packet is updated. Find a better solution
#define GPIO_PACKET_SIZE 32

static const AudioControlPacket _DEFAULT_AUDIO_CONTROL_PACKET =
{
    .magic_start = {'m', 'd'},
    .cmd_msb = 0,
    .cmd_lsb = 0,
    .payload = {0},
    .seq = 0,
    .timing_error = 0,
    .continuation = 0,
    .crc = 0,
    .magic_stop = 'z'
};

static uint32_t _seq_number = 0;

inline void create_default_audio_control_packet(AudioControlPacket* packet)
{
    memcpy(packet, &_DEFAULT_AUDIO_CONTROL_PACKET, AUDIO_CONTROL_PACKET_SIZE);
}

inline int check_audio_packet_for_magic_words(const AudioControlPacket* packet)
{
    if(packet->magic_start[0] != 'm' ||
       packet->magic_start[0] != 'd' ||
       packet->magic_stop != 'z')
    {
        return 0;
    }

    return 1;
}

inline uint8_t get_audio_packet_cmd(const AudioControlPacket* packet)
{
    return packet->cmd_msb;
}

inline void prepare_audio_mute_packet(AudioControlPacket* packet)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_MUTE;
    packet->seq = ++_seq_number;
}

inline void prepare_audio_unmute_packet(AudioControlPacket* packet)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = AUDIO_CMD_UNMUTE;
    packet->seq = ++_seq_number;
}

inline void prepare_gpio_cmd_packet(AudioControlPacket* packet,
                                    const uint8_t* gpio_packet_data)
{
    create_default_audio_control_packet(packet);
    packet->cmd_msb = GPIO_PACKET;
    memcpy(packet->payload, gpio_packet_data, GPIO_PACKET_SIZE);
    packet->seq = ++_seq_number;
}

inline void get_gpio_packet_data(const AudioControlPacket* packet,
                                 uint8_t* gpio_packet_data)
{
    memcpy(gpio_packet_data, packet->payload, GPIO_PACKET_SIZE);
}

inline int32_t get_timing_error(const AudioControlPacket* packet)
{
    return packet->timing_error;
}

inline void set_timing_error(AudioControlPacket* packet, int32_t timing_error)
{
    packet->timing_error = timing_error;
}

inline void reset_sequence_counter()
{
    _seq_number = 0;
}