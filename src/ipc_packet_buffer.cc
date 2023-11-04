#include "ipc_packet_buffer.hh"
#include <chrono>

namespace 
ecgm
{

constexpr auto timeout = std::chrono::milliseconds(100);

ipc_packet_buffer::ipc_packet_buffer()
:sem(0) {
}

std::optional<ipc_packet>
ipc_packet_buffer::pop()
{
    if(!this->sem.try_acquire_for(timeout))
    {
        return std::nullopt;
    }


    auto packet = this->queue.front();
    this->queue.pop();
    
    return std::optional<ipc_packet>{ packet };
}

void
ipc_packet_buffer::push(ipc_packet packet)
{
    this->queue.push(packet);
    this->sem.release();
}

}
