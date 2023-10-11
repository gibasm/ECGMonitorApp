#ifndef IPC_UNIX_SOCKET_HH 
#define IPC_UNIX_SOCKET_HH
#ifdef __unix__

#include <sys/un.h>
#include "ipc.hh"
#include <forward_list>

namespace
ecgm
{

class
ipc_unix_client_socket;

class
ipc_unix_server_socket : public ipc_server_socket_iface
{
public:
    ipc_unix_server_socket(const char* name, size_t max_clients=1UL);

    ipc_unix_server_socket(int fd);

    virtual
    ~ipc_unix_server_socket();

    virtual socket_ptr
    accept_connection() override;

    virtual void
    listen() override;

    virtual void
    send(void* data, size_t size) override;

    virtual size_t 
    receive(void* buf, size_t size) override;

private:
    int fd;
    sockaddr_un addr;
    const char* name;
    size_t max_clients;

    std::forward_list<ipc_unix_client_socket> client_sockets;
};

class
ipc_unix_client_socket : public ipc_client_socket_iface
{
public:
    ipc_unix_client_socket(const char* name);

    ipc_unix_client_socket(int fd);

    virtual
    ~ipc_unix_client_socket();

    virtual void 
    connect() override;

    virtual void
    send(void* data, size_t size) override;

    virtual size_t 
    receive(void* buf, size_t size) override;

private:
    int fd;
    const char* name;
    sockaddr_un addr;
};

}

#endif /* __unix__ */
#endif /* IPC_UNIX_SOCKET_HH */
