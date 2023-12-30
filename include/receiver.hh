#ifndef RECEIVER_HH
#define RECEIVER_HH

#include "ipc.hh"
#include "ipc_packet_buffer.hh"
#include "ipc_client.hh"
#include "ipc_server.hh"
#include <atomic>

namespace
ecgm
{

typedef struct
{ 
    ecgm::ipc_packet_buffer* recvbuf;
    union
    {
        ecgm::ipc_client* client;
        ecgm::ipc_server* server;
    } model;

    ipc_model_type type;
} receiver_thread_args_t;

void
receiver_thread_start(receiver_thread_args_t args);

}

#endif /* RECEIVER_HH */
