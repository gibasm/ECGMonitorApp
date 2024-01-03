#include "file_writer.hh"
#include <cstring>
#include <stdexcept>
#include "log.hh"

namespace
ecgm
{

file_writer::file_writer(const char* file_name, std::vector<float>& samples)
:samples(samples)
{
    fp = fopen(file_name, "w");
    if(fp == NULL)
        throw std::runtime_error(strerror(errno));
    log_debug("File %s opened", file_name);
}

file_writer::~file_writer()
{
    if(fp != NULL)
    {
        fflush(fp);
        fclose(fp);
        log_debug("File closed");
    }

}

void
file_writer::notify(void* arg)
{
    if(arg == nullptr)
        return;
    
    size_t point_idx = *(int*)(arg);
    log_debug("FW: Recevied notification %d", point_idx);

    notify_lock.lock();
    float sample = samples.at(point_idx);
    fprintf(fp, "%f\n", sample);
    notify_lock.unlock();
}


}
