#ifndef SERIAL_HH
#define SERIAL_HH

#include <inttypes.h>
#include <stddef.h>

namespace 
ecgmd
{

void* 
serial_open(const char* path);

void 
serial_send(void* device, uint8_t* data, size_t len);

size_t 
serial_recv(void* device, uint8_t* data, size_t len);

void 
serial_close(void* device);

}

#endif /* SERIAL_HH */
