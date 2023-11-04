#include "ipc_server.hh"
#include "ipc_unix_socket.hh"
#include "ipc.hh"
#include "log.hh"
#include <chrono>
#include <cmath>
#include <thread>
#include "arg_parser.hh"
#include <cstdlib>

using namespace ecgm;

constexpr const char* help_msg =
"ECG Monitor daemon demo\n\r"
"-h\tprint this message and exit \n\r"
"-v\tset log level to debug\n\r"
"--file\t<path to a text file with ecg samples>\n\r";

constexpr auto T = std::chrono::microseconds(2777); // signal period
                                                    // f ~ 360 Hz
int 
main(int argc, char** argv)
{               
    if(get_bool_opt("-h", argc, argv))
    {
        log_error("\n\r%s", help_msg);
        return 0;
    }

    if(!get_bool_opt("--file", argc, argv))
    {
        log_error("--file\tThis parameter is required");
        return 1;
    }

    char* file_name = nullptr;
    file_name = get_str_opt("--file", argc, argv);

    if(file_name == nullptr)
    {
        log_error("--file\tThis parameter is required");
        return 1;
    }

    if(get_bool_opt("-v", argc, argv))
    {
        set_log_lvl(log_lvl::DEBUG);
    }

    FILE* f = fopen(file_name, "r");
    if(f == NULL)
    {
        log_error("Cannot open a file at path: %s", file_name);
        return 1;
    } 
    
    std::vector<float> samples; 
    float sample;
    while(!feof(f))
    {
        fscanf(f, "%f", &sample);
        samples.push_back(sample);
    }

    fclose(f);

#ifdef __unix__
    ipc_unix_server_socket socket(APP_DEFAULT_IPC_SERVER_NAME);
#else 
    #error "This daemon demo does is not supported on any other platform than GNU/Linux"
#endif 

    ipc_server server(&socket);

reset:
    log_status("Waiting for the frontend to connect...");

    server.wait_for_connection();

    log_status("Connected.");

    while(1)
    {
        try
        {
            auto packet = server.receive(); 

            if(packet.any.type == ipc_packet_type::START)
            {

                ipc_packet ack = {
                    .any = {
                        .type = ipc_packet_type::ACK,
                        .length = sizeof(ipc_any_packet)
                    }
                };
                server.send(ack);

                log_status("Got start request, starting...");
            }


            for(uint32_t i = 0;; ++i)
            {
                ipc_packet sample = {
                    .any = {
                        .type = ipc_packet_type::ECG_SAMPLE,
                        .length = sizeof(ipc_ecg_sample_packet)
                    }
                };

                sample.sample.sample_no = i;
                sample.sample.value = samples.at(i % samples.size());
                
                server.send(sample);
                std::this_thread::sleep_for(T);        
                
            }

        }
        catch(const ipc_exception& exception)
        {
            log_error("%s", exception.what());
            log_status("Disconnected.");
            goto reset; 
        }
        catch(...)
        {
            log_error("Unexpected exception occured, exiting...");
            return 1;
        }
    }
    
    return 0;
}
