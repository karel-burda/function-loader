#pragma once

#include <utility>

#include "function_loader/exceptions.hpp"
#include "function_loader/detail/library_loader_platform_specific.hpp"

namespace burda
{
namespace function_loader
{
namespace detail {
class library_loader : public detail::library_loader_platform_specific
{
public:
    explicit library_loader(const std::string & path)
    {
        load_library(path);

        if (get_handle() == nullptr)
        {
            throw exceptions::library_load_failed{ path  + ", reason: " + get_last_error() };
        }
    }

    ~library_loader()
    {
        unload_library();
    }

    library_loader(library_loader && source)
    {
        handle = source.handle;
        source.handle = nullptr;
    }

    library_loader & operator=(library_loader && source)
    {
        if (this != &source)
        {
            unload_library();
            std::swap(handle, source.handle);
        }

        return *this;
    }
};
}
}
}
