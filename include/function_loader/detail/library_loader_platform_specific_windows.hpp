#pragma once

#include <windows.h>

#include "function_loader/detail/library_loader_base.hpp"

namespace burda
{
namespace function_loader
{
namespace detail
{
class library_loader_platform_specific : public library_loader_base<HMODULE>
{
protected:
    void load_library(const std::string & path)
    {
        handle = LoadLibraryExA(path.c_str(), nullptr, 0);
    }

    void unload_library()
    {
        if (handle != nullptr)
        {
            FreeLibrary(handle);

            handle = nullptr;
        }
    }

    std::string get_last_error() const
    {
        const auto error_code_from_os = GetLastError();

        return error_code_from_os != 0 ? std::to_string(error_code_from_os) : "";
    }
};
}
}
}
