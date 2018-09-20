#pragma once

#include<string>

#include <dlfcn.h>

namespace burda
{
namespace function_extractor
{
namespace detail
{
class function_loader_base
{
protected:
    void * get_function_address(void * handle, const std::string & function_name)
    {
        return dlsym(handle, function_name.c_str());
    }
};
}
}
}
