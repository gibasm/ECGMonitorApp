#ifdef __unix__
#include "ipc_unix_socket.hh"
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>

namespace
ecgm 
{

ipc_unix_socket::ipc_unix_socket(const char* name)
:name(name), bind_called(false)
{
    sockfd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    IPC_SOCKET_ASSERT(sockfd != -1, "Cannot create a unix domain socket");

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, name, sizeof(addr.sun_path) - 1);    
}

ipc_unix_socket::~ipc_unix_socket()
{
    ::close(sockfd);

    /* if bind was called it might be a good idea to unlink */
    if(bind_called)
        ::unlink(name);
}

void
ipc_unix_socket::init_server()
{
    IPC_SOCKET_ASSERT(::unlink(name) != -1, "Failed to unlink socket");
    IPC_SOCKET_ASSERT(::bind(sockfd, (const struct sockaddr*) &addr, sizeof(addr)) != -1, "Failed to bind the socket to the name");
    bind_called = true;
}

void 
ipc_unix_socket::allow_connections(size_t no) 
{
    IPC_SOCKET_ASSERT(no <= static_cast<size_t>(INTMAX_MAX),
        "Number for allowed connections for unix socket cannot be greater than INTMAX");

    ::listen(sockfd, static_cast<int>(no));
}

void
ipc_unix_socket::accept_connection()
{
    IPC_SOCKET_ASSERT(::accept(sockfd, NULL, NULL) != -1, 
        "Error while accepting an incoming connection");
}

void 
ipc_unix_socket::write(void* data, size_t size) 
{
    IPC_SOCKET_ASSERT(static_cast<size_t>(::write(sockfd, data, size)) == size, 
        "Error while writing to a socket!");
}

size_t 
ipc_unix_socket::read(void* data, size_t size) 
{
    ssize_t ret = ::read(sockfd, data, size);      

    IPC_SOCKET_ASSERT(ret >= 0, "Error while reading from a socket!");
    
    return static_cast<size_t>(ret);
}

void
ipc_unix_socket::connect()
{
    IPC_SOCKET_ASSERT(::connect(sockfd, (const struct sockaddr*) &addr, sizeof(addr)) == 0,
        "Cannot connect to a socket at a given address");
}

}

#endif /* __unix__ */
