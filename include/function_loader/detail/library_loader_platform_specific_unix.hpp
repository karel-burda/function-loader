#pragma once

#include <dlfcn.h>

namespace burda
{
namespace function_loader
{
namespace detail
{
class library_loader_base : public library_loader_base<void *>
{
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

    std::string get_last_error() const
    {
        const auto error_from_os = dlerror();

        return error_from_os != nullptr ? error_from_os : "";
    }
};
}
}
}
