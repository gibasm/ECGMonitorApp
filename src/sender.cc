#include "sender.hh"
#include "app_globl_state.hh"
#include "ipc.hh"
#include "log.hh"

namespace
ecgm
{

void
sender_thread_start(sender_thread_args_t args)
{
    log_debug("Sender is running...");

    while(!app_state.should_terminate)
    {
        try 
        {
            auto packet_opt = args.sendbuf->pop();

            if(packet_opt.has_value())
            {
                auto packet = packet_opt.value();
                log_debug("Sending %02x %02x", packet.any.type,
                        packet.any.length);
                args.type == IPC_CLIENT ? args.model.client->send(packet) : 
                    args.model.server->send(packet);   
            }
        }
        catch(const ipc_socket_exception& exception)
        {
            log_error("Error while sending packet: %s",
                    exception.what());
        }
    } 
}

}
