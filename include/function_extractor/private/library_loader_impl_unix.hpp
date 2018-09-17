#pragma once

#include <string>

#include <dlfcn.h>

namespace burda
{
namespace function_extractor
{
class library_loader_implementation
{
protected:
    void load_library(const std::string & path)
    {
        handle = dlopen(path.c_str(), 0);
    }

    void unload_library()
    {
        dlclose(handle);
    }

    void * handle = nullptr;
};
}
}
