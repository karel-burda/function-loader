#pragma once

#include <windows.h>

#include "function_extractor/private/library_loader_interface.hpp"

namespace burda
{
namespace function_extractor
{
class library_loader_implementation : public library_loader_interface
{
protected:
    void load_library(const std::string & path) override
    {
        handle = LoadLibraryExA(path.c_str(), nullptr, 0);
    }

    void unload_library() override
    {
        FreeLibrary(handle);
    }

    HMODULE handle = nullptr;
};
}
}
