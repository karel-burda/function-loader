#pragma once

#include <string>

#include <windows.h>

#include "function_loader/detail/function_loader_base.hpp"

namespace burda
{
namespace function_loader
{
namespace detail
{
class function_loader_platform_specific : public function_loader_base
{
public:
    using function_loader_base::function_loader_base;

protected:
    void * get_function_address(const std::string & function_name) const
    {
        library.throw_if_handle_is_invalid();

        return GetProcAddress(library.get_handle(), function_name.c_str());
    }
};
}
}
}
