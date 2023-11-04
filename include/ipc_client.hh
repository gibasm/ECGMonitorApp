#ifndef IPC_CLIENT_HH
#define IPC_CLIENT_HH

#include <mutex>
#include "ipc.hh"
#include "ipc_proto.hh"

namespace
ecgm
{

class
ipc_client
{
public:
    ipc_client(ipc_client_socket_iface* socket);

    virtual
    ~ipc_client() = default;

    void 
    connect();

    void
    send(const ipc_packet& packet);

    ipc_packet
    receive();

private:
    std::mutex lock;
    ipc_client_socket_iface* socket;

    serialized_ipc_packet recvbuf;
};

}

#endif /* IPC_CLIENT_HH */
