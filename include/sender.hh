#ifndef SENDER_HH
#define SENDER_HH

#include "ipc_packet_buffer.hh"
#include "ipc_client.hh"

namespace
ecgm
{

typedef struct 
{
    ecgm::ipc_packet_buffer* sendbuf;
    ecgm::ipc_client* client;
} sender_thread_args_t;

void
sender_thread_start(sender_thread_args_t* args);

}

#endif /* SENDER_HH */
