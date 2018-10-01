#pragma once

#include<string>

#include <windows.h>

namespace burda
{
namespace function_loader
{
namespace detail
{
class function_loader_base
{
protected:
    void * get_function_address(HMODULE handle, const std::string & function_name) const
    {
        return GetProcAddress(handle, function_name.c_str());
    }
};
}
}
}
