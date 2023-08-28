#include "ipc_server.hh" 
#include "ipc_proto.hh"
#include "ipc_unix_socket.hh"

namespace
ecgm
{

ipc_server::ipc_server(socket_ptr socket)
:socket(socket)
{
    socket->lock();

    this->socket->allow_connections(max_allowed_connections);

    socket->unlock();
}

void
ipc_server::wait_for_connection()
{
    socket->lock();
    socket->accept_connection();
    socket->unlock();
}

void
ipc_server::send(const ipc_packet& packet)
{
    auto serialized = serialize_ipc_packet(packet);

    socket->lock();
    socket->write((void*)serialized.data(), serialized.size()); 
    socket->unlock();
}

ipc_packet
ipc_server::receive()
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
