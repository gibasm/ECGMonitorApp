#ifndef SENDER_HH
#define SENDER_HH

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
    ecgm::ipc_packet_buffer* sendbuf;

    union
    {
        ecgm::ipc_client* client;
        ecgm::ipc_server* server;
    } model;

    ipc_model_type type;
} sender_thread_args_t;

void
sender_thread_start(sender_thread_args_t args);

}

#endif /* SENDER_HH */
