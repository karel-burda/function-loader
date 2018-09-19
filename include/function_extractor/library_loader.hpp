#pragma once

#include "function_extractor/exceptions.hpp"
#include "function_extractor/private/library_loader_impl.hpp"

namespace burda
{
namespace function_extractor
{
class library_loader : public library_loader_implementation
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

