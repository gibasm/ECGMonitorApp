#ifndef IPC_UNIX_SOCKET_HH 
#define IPC_UNIX_SOCKET_HH
#ifdef __unix__

#include <sys/un.h>
#include "ipc.hh"

namespace
ecgm
{

class
ipc_unix_socket :public ipc_socket_iface
{
public:
    ipc_unix_socket(const char* name);

    virtual 
    ~ipc_unix_socket();

    virtual void
    init_server() override;

    virtual void
    connect() override;

    virtual void 
    allow_connections(size_t no) override; 
   
    virtual void
    accept_connection() override;

    virtual void 
    write(void* data, size_t size) override;
    
    virtual size_t
    read(void* buf, size_t size) override;

private:
    int sockfd;
    sockaddr_un addr;
    const char* name;
    bool bind_called;
};

}

#endif /* __unix__ */
#endif /* IPC_UNIX_SOCKET_HH */
