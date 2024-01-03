#ifndef COMM_HH
#define COMM_HH

#include <inttypes.h>
#include <stddef.h>
#include "ipc_packet_buffer.hh"
#include <thread>
#include <semaphore.h>

namespace ecgmd
{

typedef struct
{
    void (*send)(uint8_t*, size_t);
    size_t (*recv)(uint8_t*, size_t);
    ecgm::ipc_packet_buffer* rxbuf, *txbuf; 
    std::atomic_bool* dev_connected;
} comm_context;

class
comm_controller
{
public:
    comm_controller(comm_context ctx);
    
    virtual ~comm_controller() = default;

    void 
    run();

private:    
    void 
    forward_samples();

    void
    process_incoming_ipc();

    void
    read_samples();

    enum 
    {
        IDLE,
        RUNNING,
    } state;
    
    comm_context ctx;
    
    uint8_t sample_buffer[1024];
    sem_t sbuf_sem;
    size_t rpos, wpos;

    std::jthread reader;
};

};

#endif /* COMM_HH */
