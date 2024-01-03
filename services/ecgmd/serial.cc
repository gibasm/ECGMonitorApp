#include "serial.hh"
#include "log.hh"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <cstdio>
#include <stdexcept>
#include <thread>
#include <queue>

namespace
ecgmd
{

#ifdef __unix__


#define SERIAL_ASSERT(x) \
    if(!(x))\
    { \
        throw std::runtime_error((const char*)strerror(errno)); \
    } \

typedef struct
{
    int fd;
    termios tty;
} serial_dev_t;


void*
serial_open(const char* path)
{
    serial_dev_t* device = new serial_dev_t({
        .fd = open(path, O_RDWR | O_NOCTTY),
        .tty = {}
    });
    
    SERIAL_ASSERT(device->fd != -1);
    SERIAL_ASSERT(tcgetattr(device->fd, &(device->tty)) == 0);
   
    device->tty.c_cflag = CS8;
    device->tty.c_cc[VMIN] = 1;
    device->tty.c_cc[VTIME] = 0;

    device->tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                    INLCR | PARMRK | INPCK | ISTRIP | IXON);

    device->tty.c_oflag = 0;
    device->tty.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    cfsetispeed(&(device->tty), B9600);
    cfsetospeed(&(device->tty), B9600);

    SERIAL_ASSERT(tcsetattr(device->fd, TCSANOW, &(device->tty)) == 0);

    return (void*)device; 
}

void
serial_send(void* device, uint8_t* data, size_t len)
{
    serial_dev_t* dev = (serial_dev_t*) device;
    SERIAL_ASSERT(write(dev->fd, data, len) != -1);
}

size_t 
serial_recv(void* device, uint8_t* data, size_t len)
{
    serial_dev_t* dev = (serial_dev_t*) device;
    return static_cast<size_t>(read(dev->fd,  data, len));
}

void
serial_close(void* device)
{
    if(device != nullptr)
    {
        serial_dev_t* dev = (serial_dev_t*) device;
        close(dev->fd);
        delete dev;
    }
}

bool
is_open(void* device)
{
    serial_dev_t* dev = (serial_dev_t*) device;
    return fcntl(dev->fd, F_GETFD) != -1 || errno != EBADF; 
}

#elif __defined(__WIN32)    
    // TODO: add Windows support
    #error "(WIP)"
#endif /* __unix__ */

}
