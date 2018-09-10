#pragma once

#include <dlfcn.h>

#include "function_extractor/private/library_loader_interface.hpp"

namespace burda
{
namespace function_extractor
{
class library_loader_implementation : public library_loader_interface
{
protected:
    void load_library(const std::string & path) override
    {
        handle = dlopen(path.c_str(), 0);
    }

    void unload_library() override
    {
        dlclose(handle);
    }

private:
    void * handle = nullptr;
};
}
}
