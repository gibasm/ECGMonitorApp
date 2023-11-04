#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include "lineplot.hh"
#include "ipc_packet_buffer.hh"

namespace
ecgm
{

class 
controller
{
public:
    controller(observer* plot, std::vector<float>& samples, ipc_packet_buffer& sendbuf, ipc_packet_buffer& recvbuf); 
    
    virtual
    ~controller() = default;

    void 
    run();

private:

    void 
    start();

    void 
    stop();

    void 
    process_incoming_packets();

    void
    wait_for_ack();

    void 
    push_packet(ipc_packet packet);

    std::optional<ipc_packet>
    pop_packet();

    enum
    state_t
    {
        IDLE,
        AWAITING_ACK,
        RUNNING
    };

    state_t state;
    observer* plot;
    std::vector<float>& samples;
    ipc_packet_buffer& sendbuf;
    ipc_packet_buffer& recvbuf;
};

}

#endif /* CONTROLLER_HH */
