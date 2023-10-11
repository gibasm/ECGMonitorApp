#include "ipc_server.hh" 
#include "ipc_proto.hh"
#include "ipc_unix_socket.hh"

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
    
    this->lock.lock();
    this->client->send((void*)serialized.data(), serialized.size());
    this->lock.unlock();
}

ipc_packet
ipc_server::receive()
{
    uint8_t* recvptr = recvbuf.data();

    this->lock.lock();
    
    size_t ret = this->client->receive(
        (void*)recvptr, ipc_min_packet_length
    );

    this->lock.unlock();

    IPC_ASSERT(ret == ipc_min_packet_length, 
        "Malformed packet arrived");

    this->lock.lock();

    recvptr += ipc_min_packet_length;

    size_t length = recvbuf[ipc_length_pos];

    this->lock.unlock();

    IPC_ASSERT(length <= ipc_max_packet_length,
        "Malformed packet arrived");

    length -= ipc_min_packet_length;
    
    if(length == 0)
        goto recv_ret;

    this->lock.lock();

    ret = this->client->receive((void*) recvptr, length);

    this->lock.unlock();
    
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
