#include "receiver.hh"
#include "app_globl_state.hh"
#include "log.hh"

namespace
ecgm
{

void
receiver_thread_start(receiver_thread_args_t args)
{
    log_debug("Receiver is running...");
    while(!app_state.should_terminate)
    {
        try 
        {
            auto packet = args.type == IPC_CLIENT ? args.model.client->receive() :
                args.model.server->receive();
            args.recvbuf->push(packet);
        }
        catch(const ipc_socket_exception& exception) 
        {
            log_error("Error while receiving a packet: %s", 
                    exception.what()); 
        }
        catch(const ipc_exception& exception)
        {
            log_debug("%s", exception.what());
            app_state.should_terminate = true;
            return;
        }
        catch(...)
        {
            log_debug("Receiver didn't expect such an exception!");
            app_state.should_terminate = true;
            return;
        }
    } 
}

}
