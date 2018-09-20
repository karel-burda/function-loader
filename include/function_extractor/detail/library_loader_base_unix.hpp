#pragma once

#include <string>

#include <dlfcn.h>

namespace burda
{
namespace function_extractor
{
namespace detail
{
class library_loader_base
{
protected:
    void load_library(const std::string & path)
    {
        handle = dlopen(path.c_str(), RTLD_NOW);
    }

    void unload_library()
    {
        dlclose(handle);
    }

    char * get_last_error()
    {
        return dlerror();
    }

    void * handle = nullptr;
};
}
}
}
