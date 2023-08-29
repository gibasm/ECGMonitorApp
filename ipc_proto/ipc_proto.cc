#include "ipc_proto.hh"
#include <cstring>
#include <stdexcept>

namespace
ecgm
{

serialized_ipc_packet 
serialize_ipc_packet(const ipc_packet& packet)
{
    IPC_ASSERT(packet.any.length >= 2U, "Packet length must be greater or equal to 2");

    serialized_ipc_packet serialized_packet;
    serialized_packet.resize(packet.any.length);

    memcpy((void*)serialized_packet.data(), (void*)&packet, packet.any.length);

    return serialized_packet;
}

constexpr const char* incorrect_packet_length_msg = "Packet length incorrect for given packet type";

ipc_packet 
deserialize_ipc_packet(const serialized_ipc_packet& serialized_packet)
{
    IPC_ASSERT(serialized_packet.size() >= 2U, "Packet length must be greater or equal to 2");
    IPC_ASSERT(serialized_packet.size() == serialized_packet[ipc_length_pos], 
               "Read packet length not matching the actual length");
 
    switch(serialized_packet.at(ipc_type_pos))
    { 
        case ipc_packet_type::ECG_SAMPLE:
        {
            IPC_ASSERT(serialized_packet.size() == sizeof(ipc_ecg_sample_packet),
                       incorrect_packet_length_msg);
            break;
        }

        case ipc_packet_type::SET_SCLKDIV:
        {
            IPC_ASSERT(serialized_packet.size() == sizeof(ipc_sclkdiv_packet),
                       incorrect_packet_length_msg);
            break;
        }

        case ipc_packet_type::START:
        case ipc_packet_type::STOP:
        case ipc_packet_type::INVALID_PACKET_TYPE:
        case ipc_packet_type::INVALID_SCLKDIV:
        case ipc_packet_type::LEADS_OFF:
        case ipc_packet_type::ALREADY_STARTED:
        case ipc_packet_type::DEVICE_NOT_RESPONSIVE:
        case ipc_packet_type::DEVICE_NOT_CONNECTED:
        {
            IPC_ASSERT(serialized_packet.size() == sizeof(ipc_any_packet),
                       incorrect_packet_length_msg);
            break;
        }

        default:
            throw ipc_exception("Invalid packet type");
    }

    ipc_packet packet;
    memcpy((void*)&packet, (void*)serialized_packet.data(), serialized_packet.size());

    return packet;
}

void 
ipc_send_packet(socket_ptr socket, const ipc_packet& packet)
{    
    auto serialized = serialize_ipc_packet(packet);

    socket->lock();
    socket->write((void*)serialized.data(), serialized.size()); 
    socket->unlock();
}

ipc_packet
ipc_receive_packet(socket_ptr socket)
{
    serialized_ipc_packet serialized;
    serialized.resize(2);
    
    size_t bytes_left = sizeof(ipc_any_packet);
    while(bytes_left != 0U)
    {
        socket->lock();
        bytes_left -= socket->read((void*)serialized.data(), sizeof(ipc_any_packet));
        socket->unlock();
    }

    bytes_left = serialized.at(ipc_length_pos) - sizeof(ipc_any_packet);
    serialized.resize(sizeof(ipc_any_packet) + bytes_left);

    size_t off = sizeof(ipc_any_packet);

    while(bytes_left != 0U)
    {
        socket->lock();
        size_t n = socket->read((void*)(serialized.data() + off), bytes_left);
        bytes_left -= n;
        off += n;
        socket->unlock();
    }

    return deserialize_ipc_packet(serialized);
}

}
