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
    library_loader(const std::string & path)
    {
        load_library(path);
    }

    ~library_loader()
    {
        unload_library();
    }
};
}
}

