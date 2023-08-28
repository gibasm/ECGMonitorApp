#ifndef IPC_HH
#define IPC_HH

#include <exception>
#include <mutex>
#include <stddef.h>

namespace
ecgm
{

constexpr const char* APP_IPC_SERVER_NAME = "/tmp/ecgm.frontend.socket";

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

    /* used by the client socket to connect */
    virtual void
    connect() = 0;

    /* set the number 'no' of allowed connections in the queue */
    virtual void 
    allow_connections(size_t no) = 0; 
   
    /* accept the first connection in the queue or wait until one's requested */
    virtual void
    accept_connection() = 0;

    /* write data to the socket */
    virtual void 
    write(void* data, size_t size) = 0;
    
    /* read the data of max 'size' bytes from the socket and return the actual number of read bytes */
    virtual size_t
    read(void* buf, size_t size) = 0;
    
    inline void
    lock()
    {
        this->lock_.lock();
    }

    inline void
    unlock()
    {
        this->lock_.unlock();
    }

private:
    std::mutex lock_;
};

}

#endif /* IPC_HH */
