#pragma once

#include "function_loader/exceptions.hpp"
#include "function_loader/detail/library_loader_base.hpp"
#include "function_loader/detail/primitives/idisable_copy.hpp"
#include "function_loader/detail/primitives/idisable_move.hpp"

namespace burda
{
namespace function_loader
{
namespace detail {
class library_loader : public detail::library_loader_base,
                       private cpp_utils::idisable_copy,
                       private cpp_utils::idisable_move
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
};
}
}
}
