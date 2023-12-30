#ifndef IPC_STACK_HH
#define IPC_STACK_HH

#include "ipc_packet_buffer.hh"
#include "ipc.hh"
#include "ipc_client.hh"
#include "ipc_server.hh"

namespace
ecgm
{

void 
ipc_stack_run(ipc_packet_buffer* sendbuf, ipc_packet_buffer* recvbuf, void* model, ipc_model_type type);

}

#endif /* IPC_STACK_HH */
