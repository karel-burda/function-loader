#pragma once

#include <string>

namespace burda
{
namespace function_extractor
{
class library_loader_interface
{
protected:
    virtual void load_library(const std::string & path) = 0;

    virtual void unload_library() = 0;
};
}
}

