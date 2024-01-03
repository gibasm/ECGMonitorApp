#include "controller.hh"
#include "app_globl_state.hh"
#include "log.hh"

namespace
ecgm
{

constexpr static const char* IPC_PROTO_ERROR_STR_LUT[] =
{
    "Invalid/unsupported packet type",
    "Invalid sclk divider value",
    "Leads off.", 
    "Device already started. Ignoring repeated start condition",
    "Device not responding to messages.",
    "Device appears to be disconnected, connect the device."
};

#define IPC_PROTO_LOOKUP_ERROR_STR(ptype) \
    (IPC_PROTO_ERROR_STR_LUT[ptype & (~ipc_packet_error_bit)])

controller::controller(observer** observers, size_t n_observers, std::vector<float>& samples, ipc_packet_buffer& sendbuf, ipc_packet_buffer& recvbuf)
:state(controller::state_t::IDLE), observers(observers), n_observers(n_observers), samples(samples), sendbuf(sendbuf), recvbuf(recvbuf)
{
} 

void
controller::run()
{
    this->start();

    while(!app_state.should_terminate)
    {  
        switch(state)
        {
            case controller::state_t::RUNNING:
            {
                this->process_incoming_packets();
                break;
            }

            case controller::state_t::AWAITING_ACK:
            {
                this->wait_for_ack();
                break;
            }

            case controller::state_t::IDLE:
            default:
            {
                break;
            }
        }
    }
}

void
controller::start()
{
    ipc_packet packet = {
        .any = {
            .type = START,
            .length = ipc_min_packet_length 
        }
    }; 

    this->push_packet(packet);

    this->state = controller::state_t::AWAITING_ACK;
}

void
controller::stop()
{
    ipc_packet packet = {
        .any = {
            .type = STOP,
            .length = ipc_min_packet_length
        }
    };

    this->push_packet(packet);
    this->state = controller::state_t::IDLE;
}

void
controller::process_incoming_packets()
{
    auto packet_opt = this->pop_packet();
    if(!packet_opt.has_value())
        return;

    auto packet = packet_opt.value();

    if(IPC_PACKET_ERROR_BIT_SET(packet))
    {
        log_error("%s", IPC_PROTO_LOOKUP_ERROR_STR(packet.any.type)); 
        return;
    }

    switch(packet.any.type)
    {
        case ipc_packet_type::ECG_SAMPLE:
        {
            int idx = packet.sample.sample_no % this->samples.size();
            this->samples.at(idx) = packet.sample.value; 
            for(size_t i = 0; i < n_observers; ++i)
            {
                if(this->observers[i] != nullptr)
                    this->observers[i]->notify(&idx);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void
controller::wait_for_ack()
{
    auto packet_opt = this->pop_packet();
    if(!packet_opt.has_value())
        return;

    auto packet = packet_opt.value();

    if(IPC_PACKET_ERROR_BIT_SET(packet))
    {
        log_error("%s", IPC_PROTO_LOOKUP_ERROR_STR(packet.any.type)); 
        return;   
    }

    if(packet.any.type == ipc_packet_type::ACK)
    {
        log_status("ACK received, the device is running.");
        this->state = controller::state_t::RUNNING;   
    }
}

void
controller::push_packet(ipc_packet packet)
{
    this->sendbuf.push(packet);
}

std::optional<ipc_packet>
controller::pop_packet()
{
    return this->recvbuf.pop();
}

}
