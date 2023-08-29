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
    ipc_send_packet(this->socket, packet);
}

ipc_packet
ipc_server::receive()
{
    return ipc_receive_packet(this->socket); 
}

}
