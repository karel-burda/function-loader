#pragma once

#include <string>

#include <windows.h>

namespace burda
{
namespace function_extractor
{
class library_loader_implementation
{
protected:
    void load_library(const std::string & path)
    {
        handle = LoadLibraryExA(path.c_str(), nullptr, 0);
    }

    void unload_library()
    {
        FreeLibrary(handle);
    }

    const char * get_last_error()
    {
        last_error = std::to_string(GetLastError());

        return last_error.c_str();
    }

    HMODULE handle = nullptr;

private:
    std::string last_error;
};
}
}
