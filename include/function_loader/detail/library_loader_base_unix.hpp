#pragma once

#include <string>

#include <dlfcn.h>

namespace burda
{
namespace function_loader
{
namespace detail
{
class library_loader_base
{
public:
    void * get_handle() const
    {
        return handle;
    }

protected:
    void load_library(const std::string & path)
    {
        handle = dlopen(path.c_str(), RTLD_NOW);
    }

    void unload_library()
    {
        if (handle)
        {
            dlclose(handle);

            handle = nullptr;
        }
    }

    char * get_last_error()
    {
        return dlerror();
    }

private:
    void * handle = nullptr;
};
}
}
}
