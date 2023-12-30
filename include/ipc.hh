#ifndef IPC_HH
#define IPC_HH

#include <exception>
#include <stddef.h>

namespace
ecgm
{

constexpr const char* APP_DEFAULT_IPC_SERVER_NAME = "/tmp/ecgmd.socket";

class 
ipc_exception : public std::exception
{
public:
    explicit 
    ipc_exception(const char* message)
        :msg_(message) {}

    virtual 
    ~ipc_exception() noexcept = default;

    virtual const char* 
    what() const noexcept override 
    { 
        return msg_; 
    } 
private:
    const char* msg_; 
};

#define IPC_ASSERT(x, msg) \
    { if(!(x)) throw ipc_exception(msg); }

class
ipc_socket_exception : public ipc_exception
{
public:
    explicit
    ipc_socket_exception(const char* message)
        :ipc_exception(message) {}
};

#define IPC_SOCKET_ASSERT(x, msg) \
    { if(!(x)) throw ipc_socket_exception(msg); }

class
ipc_socket_iface
{
public:
    /* @brief Send (blocking). 
     * @param data pointer to data to be send
     * @param size size of the mentioned data */
    virtual void 
    send(void* data, size_t size) = 0;
    
    /* @brief Receive (blocking).
     * @param buf pointer to the receive buffer
     * @param size size of the buffer 
     * @return size_t retuns the size of the received data
     *
     * If received data size is larger than 
     * the buffer size an exception is thrown. */
    virtual size_t
    receive(void* buf, size_t size) = 0;
};

typedef ipc_socket_iface* socket_ptr;

class
ipc_server_socket_iface : ipc_socket_iface
{
public:
    /* @brief Accept next pending connection from the client (blocking).
     * @return pointer to the client socket */
    virtual socket_ptr
    accept_connection() = 0; 

    /* @brief Start listening for connections (nonblocking). */
    virtual void
    listen() = 0;
};

class 
ipc_client_socket_iface : ipc_socket_iface
{
public:
    /* @breif Connect to the previously specified socket
     * @return pointer to the server socket */
    virtual void 
    connect() = 0;  
};

enum
ipc_model_type
{
    IPC_SERVER = 0,
    IPC_CLIENT = 1,
};

}

#endif /* IPC_HH */
