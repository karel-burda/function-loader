#pragma once

#include "function_extractor/exceptions.hpp"
#include "function_extractor/detail/library_loader_base.hpp"

namespace burda
{
namespace function_extractor
{
namespace detail {
class library_loader : public detail::library_loader_base
{
public:
    explicit library_loader(const std::string & path)
    {
        load_library(path);

        if (handle == nullptr)
        {
            throw exceptions::library_load_failed{ path  + ", reason: " + get_last_error() };
        }
    }

    ~library_loader()
    {
        unload_library();
    }
};
}
}
}
