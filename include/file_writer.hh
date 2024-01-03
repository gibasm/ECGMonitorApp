#ifndef FILE_WRITER_HH
#define FILE_WRITER_HH

#include "observer.hh"
#include <vector>
#include <cstdio>
#include <mutex>

namespace
ecgm
{

class
file_writer :public observer
{
public:
    file_writer(const char* file_name, std::vector<float>& samples);

    virtual 
    ~file_writer();

    virtual void 
    notify(void* arg) override;

private:
    FILE* fp;
    std::vector<float>& samples;
    std::mutex notify_lock;
};

}

#endif /* FILE_WRITER_HH */
