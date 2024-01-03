#include "comm.hh"
#include "app_globl_state.hh"
#include "ipc_proto.hh"
#include "log.hh"
#include <chrono>
#include <semaphore.h>
#include <stdexcept>
#include <memory.h>
#include <errno.h>

using namespace ecgm;

namespace
ecgmd
{

comm_controller::comm_controller(comm_context ctx)
:ctx(ctx), reader(&comm_controller::read_samples, this)
{
    this->state = IDLE;
    sem_init(&sbuf_sem, 0, 0);    
    rpos = 0; wpos = 0;

    reader.detach();
}

void
comm_controller::run()
{
    process_incoming_ipc();
    forward_samples();
}

void
comm_controller::read_samples()
{
    while(!app_state.should_terminate)
    {
        if(!*ctx.dev_connected)
            continue;

        size_t ret = ctx.recv((sample_buffer + wpos), 1);

        if(ret == 1) 
        {
            wpos = (wpos  + 1) % 1024;
            sem_post(&sbuf_sem);
        }
    }
}

void
comm_controller::process_incoming_ipc()
{
    if(ctx.rxbuf->empty())
        return;

    auto opt = ctx.rxbuf->pop(); 
    if(!opt.has_value())
        return;
   
    auto packet = opt.value(); 

    switch(packet.any.type)
    {
        case ipc_packet_type::START:
        {
            if(this->state == IDLE) 
            {
                log_debug("Start received, starting..."); 

                ipc_packet ipc_pack;
                ipc_pack.any.length = sizeof(ipc_any_packet);

                if(*ctx.dev_connected)
                {
                    ipc_pack.any.type = ipc_packet_type::ACK;
                }
                else
                {
                    ipc_pack.any.type = ipc_packet_type::DEVICE_NOT_CONNECTED;
                }

                ctx.txbuf->push(ipc_pack);

                this->state = RUNNING;  
            } 
            break; 
        }    

        case ecgm::ipc_packet_type::STOP:
        {
            if(this->state == RUNNING)
            {
                log_debug("Stop received, stopping..."); 

                ipc_packet ipc_pack;
                ipc_pack.any.length = sizeof(ipc_any_packet);

                if(*ctx.dev_connected)
                {
                    ipc_pack.any.type = ipc_packet_type::ACK;
                }
                else
                {
                    ipc_pack.any.type = ipc_packet_type::DEVICE_NOT_CONNECTED;
                }

                ctx.txbuf->push(ipc_pack);

                this->state = IDLE;
            }

            break;
        }
    }
}

void
comm_controller::forward_samples()
{
    if(state != RUNNING)
        return;
    if(!*ctx.dev_connected)
        return;

    static uint32_t sample_no = 0;

    uint8_t data[2];

    sem_wait(&sbuf_sem);
    data[0] = *(sample_buffer + rpos);
    rpos = (rpos + 1) % 1024;

    sem_wait(&sbuf_sem);
    data[1] = *(sample_buffer + rpos);
    rpos = (rpos + 1) % 1024;
    
    if(data[1] & 0xF0) {
        data[0] = data[1];
        sem_wait(&sbuf_sem);

        data[1] = *(sample_buffer + rpos);
        rpos = (rpos + 1) % 1024;
    }

    uint16_t sample = (data[1] << 8) | data[0];

    log_debug("0x%04x", sample);

    ipc_packet sample_packet = {
        .any =  {
            .type = ipc_packet_type::ECG_SAMPLE,
            .length = sizeof(ipc_ecg_sample_packet),
        },
    };

    sample_packet.sample.sample_no = sample_no++;
    sample_packet.sample.value = (sample / (4095.0f)) - 0.5f;

    ctx.txbuf->push(sample_packet);

    log_debug("Forwarding %f", sample_packet.sample.value);
}

};
