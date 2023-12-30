#include "hw_proto.hh"
#include "ipc_proto.hh"
#include "ipc_server.hh"
#include "ipc_unix_socket.hh"
#include "ipc.hh"
#include "log.hh"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <thread>
#include "arg_parser.hh"
#include "serial.hh"
#include "ipc_stack.hh"
#include <cstdlib>
#include "app_globl_state.hh"
#include "ipc_proto.hh"
#include "comm.hh"

using namespace ecgm;
using namespace ecgmd;

constexpr const char* help_msg =
"ECG Monitor daemon \n\r"
"-h\tprint this message and exit \n\r"
"-v\tset log level to debug\n\r"
"-s\tsilent mode\n\r"
"--frontend-sockn <socket name>\tuse provided socket name instead of the default one\n\r"
"--wired <device path/name>\trun this daemon for a device connected over USB/UART\n\r";

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1 
#endif /* EXIT_FAILURE */

static char* ipc_socket_name = nullptr;
static char* dev_ip = nullptr;
static char* dev_path = nullptr;

void* serial_dev;

enum 
connection_t {
    WIRED = 0,
    WIRELESS = 1
};

static connection_t conn_type = WIRED;

static int
parse_args(int argc, char** argv);

static ipc_packet_buffer ipc_sendbuf, ipc_recvbuf;

static void
__serial_send(uint8_t* data, size_t len);

static size_t 
__serial_recv(uint8_t* data, size_t len);

int 
wait_for_device();

int 
main(int argc, char** argv)
{               
    if(parse_args(argc, argv) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    while(true)
    {
        log_status("Trying to connect to the device...");
        if(wait_for_device() != EXIT_SUCCESS)
            return EXIT_FAILURE;
        log_status("Device connected."); 

        try
        {
            ipc_unix_server_socket socket(ipc_socket_name);
            ipc_server server(&socket);

            log_status("Waiting for the frontend to connect...");
            server.wait_for_connection();
            log_status("Connected.");

            comm_context ctx = {
                .send = __serial_send,
                .recv = __serial_recv,
                .rxbuf = &ipc_recvbuf,
                .txbuf = &ipc_sendbuf,
            };

            comm_controller controller(ctx);

            ipc_stack_run(&ipc_sendbuf, &ipc_recvbuf, &server, ipc_model_type::IPC_SERVER);

            while(!app_state.should_terminate)
            {                       
                controller.run(); 
            }
        } 
        catch(ipc_exception& e)
        {
            log_error("%s", e.what());
            serial_close(serial_dev);
            return EXIT_FAILURE;
        }
        catch(...)
        {
            log_error("Unexpected exception caught, exiting...");
            serial_close(serial_dev);
            return EXIT_FAILURE;
        }


    } /* while */

    return EXIT_SUCCESS;
}

static int
parse_args(int argc, char** argv)
{ 
    if(get_bool_opt("-h", argc, argv))
    {
        log_error("\n\r%s", help_msg);
        return EXIT_FAILURE;
    }

    if(get_bool_opt("-v", argc, argv))
    {
        set_log_lvl(log_lvl::DEBUG);
    }
    else if(get_bool_opt("-s", argc, argv))
    {
        set_log_lvl(log_lvl::SILENT);
    }
        
    conn_type = get_bool_opt("--wireless", argc, argv) ? connection_t::WIRELESS : connection_t::WIRED; 

    dev_path = get_str_opt("--wired", argc, argv);
    dev_ip = get_str_opt("--wireless", argc, argv);

    ipc_socket_name = get_str_opt("--frontend-sockn", argc, argv);
    if(ipc_socket_name == nullptr)
    {
        ipc_socket_name = (char*)APP_DEFAULT_IPC_SERVER_NAME;
    }

    return EXIT_SUCCESS;
}


static void
__serial_send(uint8_t* data, size_t len)
{
    serial_send(serial_dev, data, len);
}

static size_t 
__serial_recv(uint8_t* data, size_t len)
{
    return serial_recv(serial_dev, data, len);
}

int
wait_for_device()
{ 
    if(conn_type != connection_t::WIRED)
        return EXIT_FAILURE;

    try
    {
        serial_dev = serial_open(dev_path);   
        return EXIT_SUCCESS;
    } 
    catch(std::runtime_error& error)
    {
        log_error("%s", error.what());
        return EXIT_FAILURE;
    }
}
