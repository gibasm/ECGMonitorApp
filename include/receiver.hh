#ifndef RECEIVER_HH
#define RECEIVER_HH

#include "ipc_packet_buffer.hh"
#include "ipc_client.hh"

namespace
ecgm
{

typedef struct
{ 
    ecgm::ipc_packet_buffer* recvbuf;
    ecgm::ipc_client* client;
} receiver_thread_args_t;

void
receiver_thread_start(receiver_thread_args_t* args);

}

#endif /* RECEIVER_HH */
