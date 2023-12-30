#include "ipc_stack.hh"
#include "ipc.hh"
#include "receiver.hh"
#include "sender.hh"
#include "log.hh"

#include <thread>

namespace
ecgm
{

void 
ipc_stack_run(ipc_packet_buffer* sendbuf, ipc_packet_buffer* recvbuf, void* model, ipc_model_type type)
{ 
    log_debug("Initializing IPC stack...");
    receiver_thread_args_t rargs = {
        .recvbuf = recvbuf,
        .model = {nullptr},
        .type = type
    };

    sender_thread_args_t sargs = {
        .sendbuf = sendbuf,
        .model = {nullptr},
        .type = type
    };

    if(type == IPC_CLIENT)
    {
        rargs.model.client = (ipc_client*)model;     
        sargs.model.client = (ipc_client*)model;     
    }
    else 
    {
        rargs.model.server = (ipc_server*)model;     
        sargs.model.server = (ipc_server*)model;     
    }
    
    log_debug("Starting receiver thread...");
    std::jthread receiver_thread(receiver_thread_start, rargs);
    log_debug("Done.");

    log_debug("Starting sender thread...");
    std::jthread sender_thread(sender_thread_start, sargs);
    log_debug("Done.");

    log_debug("Detaching sender and receiver threads...");
    receiver_thread.detach();
    sender_thread.detach();
    log_debug("Done.");

    log_debug("IPC stack initialization done.");
}

}
