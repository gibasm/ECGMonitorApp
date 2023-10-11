#ifdef __unix__
#include "ipc_unix_socket.hh"
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>

namespace
ecgm 
{

inline sockaddr_un
__get_af_unix_addr(const char* name)
{
    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, name, sizeof(addr.sun_path) - 1);

    return addr; 
}

inline int 
__create_af_unix_stream_socket()
{
    int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    IPC_SOCKET_ASSERT(fd != -1, "Unable to create a socket.");
    return fd;
}

inline size_t 
__receive_blocking(int sock_fd, void* buffer, size_t size)
{
    ssize_t ret = ::recv(sock_fd, buffer, size, MSG_WAITALL); 
    IPC_SOCKET_ASSERT(ret != -1, "Receive returned an error");
    return static_cast<size_t>(ret);
}

inline void 
__send_blocking(int sock_fd, void* data, size_t size)
{
    IPC_SOCKET_ASSERT(::send(sock_fd, data, size, 0) == static_cast<ssize_t>(size),
        "Send returned an error");
}

ipc_unix_server_socket::ipc_unix_server_socket(const char* name, size_t max_clients)
:name(name), max_clients(max_clients)
{
    this->fd = __create_af_unix_stream_socket(); 
    this->addr = __get_af_unix_addr(name);

    ::unlink(name);
    IPC_SOCKET_ASSERT(::bind(
        this->fd, 
        (const struct sockaddr*) &(this->addr), 
        sizeof(addr)) != -1, "Failed to bind the socket to the name");
    
}

ipc_unix_server_socket::ipc_unix_server_socket(int fd)
:fd(fd), name(NULL), max_clients(0)
{
    
}

ipc_unix_server_socket::~ipc_unix_server_socket()
{
    if(name != NULL)
    {
        ::close(this->fd);
        ::unlink(name);
    }
}

void
ipc_unix_server_socket::listen()
{
    IPC_SOCKET_ASSERT(::listen(this->fd, max_clients) == 0,
        "Failed to call listen()");
}

socket_ptr
ipc_unix_server_socket::accept_connection()
{
    int client_sock_fd = ::accept(this->fd, NULL, NULL); 

    IPC_SOCKET_ASSERT(client_sock_fd != -1, 
        "Error while accepting an incoming connection");
    
    client_sockets.push_front(ipc_unix_client_socket(client_sock_fd));

    return (socket_ptr) &client_sockets.front(); 
}

void
ipc_unix_server_socket::send(void* data, size_t size)
{
    __send_blocking(this->fd, data, size);
}

size_t
ipc_unix_server_socket::receive(void* buf, size_t size)
{
    return __receive_blocking(this->fd, buf, size);
}

ipc_unix_client_socket::ipc_unix_client_socket(const char* name)
:name(name)
{
    this->fd = __create_af_unix_stream_socket(); 
    this->addr = __get_af_unix_addr(name);
}

ipc_unix_client_socket::ipc_unix_client_socket(int fd)
:fd(fd), name(NULL)
{
}

ipc_unix_client_socket::~ipc_unix_client_socket()
{
    if(name != NULL)
    {
        ::close(fd);
    }
}

void
ipc_unix_client_socket::connect()
{
    auto ret = ::connect(this->fd, 
            (const struct sockaddr*) &addr, sizeof(addr));
    IPC_SOCKET_ASSERT(ret != -1,
        "Cannot connect to the server with given address");
}

void 
ipc_unix_client_socket::send(void* data, size_t size)
{
    __send_blocking(this->fd, data, size);
}

size_t
ipc_unix_client_socket::receive(void* buf, size_t size)
{
    return __receive_blocking(this->fd, buf, size);
}

}

#endif /* __unix__ */
