#ifndef IPC_SERVER_HH
#define IPC_SERVER_HH 

#include <mutex>
#include "ipc.hh"
#include "ipc_proto.hh"

namespace
ecgm
{

constexpr size_t max_allowed_connections = 1UL;

class
ipc_server
{
public:
    ipc_server(ipc_server_socket_iface* socket);

    virtual
    ~ipc_server() = default;

    void 
    wait_for_connection();

    void 
    send(const ipc_packet& packet); 

    ipc_packet
    receive();

private:
    std::mutex lock;    
    ipc_server_socket_iface* socket;
    socket_ptr client;

    serialized_ipc_packet recvbuf;
};

}

#endif /* IPC_SERVER_HH */
