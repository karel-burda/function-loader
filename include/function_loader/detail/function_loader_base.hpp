#pragma once

#include <string>

#include "function_loader/detail/library_loader.hpp"

namespace burda
{
namespace function_loader
{
namespace detail
{
class function_loader_base
{
public:
    function_loader_base(const std::string & library_path)
        : library{library_path}
    {
    }

protected:
    library_loader library;
};
}
}
}
