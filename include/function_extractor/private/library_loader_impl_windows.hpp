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

    HMODULE handle = nullptr;
};
}
}
