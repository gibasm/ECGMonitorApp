#ifndef IPC_SERVER_HH
#define IPC_SERVER_HH 

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
    ipc_server(socket_ptr socket);

    virtual
    ~ipc_server() = default;

    void 
    wait_for_connection();

    void 
    send(const ipc_packet& packet); 

    ipc_packet
    receive();
private:
    socket_ptr socket;
};

}

#endif /* IPC_SERVER_HH */
