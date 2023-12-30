#include "ipc_server.hh" 
#include "ipc.hh"
#include "ipc_proto.hh"
#include <cstring>
#include "log.hh"

namespace
ecgm
{

ipc_server::ipc_server(ipc_server_socket_iface* socket)
:socket(socket)
{
    this->lock.lock();

    recvbuf.resize(ipc_max_packet_length);
    memset(recvbuf.data(), 0, ipc_max_packet_length);

    this->lock.unlock();    
}

void
ipc_server::wait_for_connection()
{
    this->lock.lock();

    this->socket->listen();
    this->client = this->socket->accept_connection();

    this->lock.unlock();
}

void
ipc_server::send(const ipc_packet& packet)
{
    auto serialized = serialize_ipc_packet(packet);
   
    this->client->send((void*)serialized.data(), serialized.size());
    log_debug("Sending: %02x %02x", packet.any.type, packet.any.length);
}

ipc_packet
ipc_server::receive()
{
    memset(recvbuf.data(), 0, ipc_max_packet_length);
    uint8_t* recvptr = recvbuf.data();
    
    size_t ret = this->client->receive(
        (void*)recvptr, ipc_min_packet_length
    );

    IPC_ASSERT(ret == ipc_min_packet_length, 
        "Malformed packet arrived");

    log_debug("Received: %02x %02x", *recvptr, *(recvptr + 1));

    recvptr += ipc_min_packet_length;

    size_t length = recvbuf[ipc_length_pos];

    IPC_ASSERT(length <= ipc_max_packet_length,
        "Malformed packet arrived");

    length -= ipc_min_packet_length;
    
    if(length == 0)
        goto recv_ret;

    ret = this->client->receive((void*) recvptr, length);

    IPC_ASSERT(ret == length,  
        "Malformed packet arrived");

recv_ret:    
    return deserialize_ipc_packet(
        serialized_ipc_packet(
            recvbuf.begin(), recvbuf.begin() + recvbuf[ipc_length_pos]
        )
    );     
}

}
