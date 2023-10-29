#ifndef IPC_PROTO_HH
#define IPC_PROTO_HH

#include <inttypes.h>
#include <vector>
#include "ipc.hh"

#define __IPC_PACKET_ATTR__ [[gnu::packed]]

namespace 
ecgm 
{

/* if MSB is set the packet signals an error */
constexpr uint8_t ipc_packet_error_bit = (1U << 7);

enum 
ipc_packet_type
{
/* normal packets */
    ECG_SAMPLE              = 0x00U,
    SET_SCLKDIV             = 0x01U, 
    START                   = 0x03U,
    STOP                    = 0x04U,
    ACK                     = 0x05U,
/* error packets */
    INVALID_PACKET_TYPE     = ipc_packet_error_bit | 0x00U,
    INVALID_SCLKDIV         = ipc_packet_error_bit | 0x01U,
    LEADS_OFF               = ipc_packet_error_bit | 0x02U,
    ALREADY_STARTED         = ipc_packet_error_bit | 0x03U,
    DEVICE_NOT_RESPONSIVE   = ipc_packet_error_bit | 0x04U,
    DEVICE_NOT_CONNECTED    = ipc_packet_error_bit | 0x05U,
};

struct __IPC_PACKET_ATTR__ ipc_any_packet
{
    uint8_t type;
    uint8_t length; 
};

struct __IPC_PACKET_ATTR__ ipc_ecg_sample_packet
{
    uint8_t  header[2];
    uint32_t sample_no;
    float    value;     /* relative value <-1.0f, 1.0f> */
};

struct __IPC_PACKET_ATTR__ ipc_sclkdiv_packet
{
    uint8_t header[2];
    uint8_t sclkdiv;    /* sampling clock divider */
}; 

union __IPC_PACKET_ATTR__ ipc_packet 
{
    ipc_any_packet        any;
    ipc_ecg_sample_packet sample;
    ipc_sclkdiv_packet    sclk;
}; 

constexpr size_t ipc_type_pos   = 0UL;
constexpr size_t ipc_length_pos = 1UL; 

constexpr size_t ipc_max_packet_length = sizeof(ipc_packet);
constexpr size_t ipc_min_packet_length = sizeof(ipc_any_packet);

typedef std::vector<uint8_t> serialized_ipc_packet;

serialized_ipc_packet 
serialize_ipc_packet(const ipc_packet& packet);

ipc_packet 
deserialize_ipc_packet(const serialized_ipc_packet& serialized_packet);

#define IPC_PACKET_ERROR_BIT_SET(packet) (((packet).any.type & ipc_packet_error_bit) != 0)

}

#endif /* IPC_PROTO_HH */
