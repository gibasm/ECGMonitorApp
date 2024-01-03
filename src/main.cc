#include <cstdlib>
#include <thread>
#include <stdexcept>
#include <vector>
#include <SDL2/SDL.h>
#include "observer.hh"
#include "renderer.hh"
#include "lineplot.hh"
#include "ipc.hh"
#include "controller.hh"
#include "app_globl_state.hh"
#include "ipc_unix_socket.hh"
#include "ipc_stack.hh"
#include "file_writer.hh"
#include <signal.h>

#include "log.hh"
#include "arg_parser.hh"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

constexpr const char* window_title = "ECG Monitor"; 
constexpr const int initial_width = 1080;
constexpr const int initial_height = 720; 
constexpr const size_t initial_sample_count = 1000;
constexpr const char* help_message =
"Usage: ./ecgm_app [-h][-vs] [--sockn] <...>\n\r"
"\tECG Monitor App (c) Michal Gibas 2023 \n\r"
"Options:                                \n\r"
"-h\tdisplay this message and exit       \n\r"
"-v\tverbose, set log level to debug     \n\r"
"-s\tsilent, set log level to error      \n\r"
"                                        \n\r"
"--sockn\t<socket name> connect to a "
"non-default ipc socket                  \n\r"
"--file\t<file path> save the signal to a"
"file in plain text                      \n\r"
"--nographic, -ng\tdon't open a window "
"and don't render any graphics           \n\r";

ecgm::file_writer* fw = nullptr;

namespace ecgm
{
extern SDL_Renderer* renderer;
}

using namespace ecgm;

static void 
sigint_handler(int);

int 
main(int argc, char** argv)
{
    signal(SIGINT, sigint_handler);

    if(get_bool_opt("-h", argc, argv)) 
    {
        printf("%s", help_message); 
        return EXIT_SUCCESS;
    }
    
    if(get_bool_opt("-v", argc, argv))
    {
        set_log_lvl(log_lvl::DEBUG);
    }

    if(get_bool_opt("-s", argc, argv))
    {
        set_log_lvl(log_lvl::ERROR);
    } 

    bool nographic = get_bool_opt("--nographic", argc, argv) || get_bool_opt("-ng", argc, argv);
    char* socket_name = get_str_opt("--sockn", argc, argv);
    char* outfile_name = get_str_opt("--file", argc, argv);
    size_t n_observers = 1;
    observer* observers[2] = { nullptr, nullptr };

    if(socket_name == nullptr)
        socket_name = (char*)APP_DEFAULT_IPC_SERVER_NAME;

    std::vector<float> samples(initial_sample_count);
    memset(samples.data(), 0, initial_sample_count * sizeof(float));

    ipc_packet_buffer sendbuf, recvbuf;

#ifdef __unix__
    ipc_unix_client_socket socket(socket_name);
#elif defined(__WIN32)
    //TODO: implement sockets using WinSock
    #error "Windows sockets are not implemented yet"
#else 
    #error "Supported platforms: GNU/Linux, Windows"
#endif 
    ipc_client client(&socket);

    log_status("Trying to connect to a ecgmd service...");

    try 
    {
        client.connect(); 
    }
    catch (const ipc_socket_exception& exception)
    {
        log_error("%s", exception.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        log_error("Unexpected exception caught, exiting...");
        return EXIT_FAILURE;
    }
   
    log_status("Connected.");

    if(outfile_name != nullptr) 
    {
        ++n_observers;
        try 
        {
            fw = new file_writer(outfile_name, samples); 
            observers[1] = fw;
        }
        catch(std::runtime_error& error)
        {
            log_error("%s", error.what());
            return EXIT_FAILURE;
        }
    }

    if(!nographic)
    {
        render_list_t render_targets;

        renderer_args_t renderer_args = {
            .render_targets = &render_targets,
            .window_title = window_title,
            .window_width = initial_width,
            .window_height = initial_height
        }; 

        log_debug("Initializing render contexts...");
        render_context main_plot_context(&renderer, 1020, 360, 20, 200);
        render_context text_context(&renderer, 1020, 360, 20, 200);
        log_debug("Done.");
       
        log_debug("Preparing ECG plot render target..."); 
        lineplot<std::vector<float>::iterator> plot(
                samples.begin(), 
                samples.end(), 
                &main_plot_context);
        plot.set_xticks(360);
        plot.set_yticks(50);

        observers[0] = &plot;

        render_targets.push_back(&plot); 
        log_debug("Done.");

        log_debug("Starting renderer thread...");
        std::jthread renderer_thread(renderer_thread_start, &renderer_args);
        log_debug("Done.");

        ipc_stack_run(&sendbuf, &recvbuf, &client, ipc_model_type::IPC_CLIENT);

        controller main_controller(observers, n_observers, samples, sendbuf, recvbuf);

        main_controller.run();

        renderer_thread.join(); 
    } 
    else 
    {
        ipc_stack_run(&sendbuf, &recvbuf, &client, ipc_model_type::IPC_CLIENT);

        controller main_controller(observers, n_observers, samples, sendbuf, recvbuf);

        main_controller.run();
    }

    if(fw != nullptr) 
        delete fw;

    return EXIT_SUCCESS;
}

static void 
sigint_handler([[maybe_unused]]int sig)
{
    if(fw != nullptr) 
        delete fw;
}
