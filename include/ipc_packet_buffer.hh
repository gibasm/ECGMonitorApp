#ifndef IPC_PACKET_BUFFER_HH
#define IPC_PACKET_BUFFER_HH

#include <queue>
#include <semaphore>
#include <optional>
#include "ipc_proto.hh"

namespace
ecgm
{

class
ipc_packet_buffer
{
public:
    ipc_packet_buffer();

    virtual
    ~ipc_packet_buffer() = default;

    std::optional<ipc_packet>
    pop();

    void
    push(ipc_packet packet);

    bool
    empty() const;

private:
    std::queue<ipc_packet> queue;
    std::counting_semaphore<INT_MAX> sem;
};

}

#endif /* IPC_PACKET_BUFFER_HH */
