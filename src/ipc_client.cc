#include "ipc_client.hh"
#include "log.hh"
#include <cstring>

namespace
ecgm
{

ipc_client::ipc_client(ipc_client_socket_iface* socket)
:socket(socket)
{
    this->lock.lock();

    recvbuf.resize(ipc_max_packet_length);
    memset(recvbuf.data(), 0, ipc_max_packet_length);

    this->lock.unlock();
}

void
ipc_client::connect()
{
    this->lock.lock();
    try 
    {
        this->socket->connect();
    }
    catch(...)
    {
        this->lock.unlock();
        throw;
    }
    
    this->lock.unlock();
}

void 
ipc_client::send(const ipc_packet& packet)
{
    auto serialized = serialize_ipc_packet(packet);

    ((socket_ptr)this->socket)->send((void*)serialized.data(), serialized.size());
}

ipc_packet
ipc_client::receive()
{
    memset(recvbuf.data(), 0, ipc_max_packet_length);
    uint8_t* recvptr = recvbuf.data();

    
    size_t ret = ((socket_ptr)this->socket)->receive(
        (void*)recvptr, ipc_min_packet_length
    );


    IPC_ASSERT(ret == ipc_min_packet_length, 
        "Malformed packet arrived");

    log_debug("Packet header received: %02x %02x", *recvptr, *(recvptr + 1));


    recvptr += ipc_min_packet_length;

    size_t length = recvbuf[ipc_length_pos];

    IPC_ASSERT(length <= ipc_max_packet_length,
        "Malformed packet arrived");

    length -= ipc_min_packet_length;
    
    log_debug("Remaining bytes to read: %d", length);

    if(length == 0)
        goto recv_ret;

    ret = ((socket_ptr)this->socket)->receive((void*) recvptr, length);
    
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
