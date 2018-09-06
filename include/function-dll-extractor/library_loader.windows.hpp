#pragma once

#include <functional>
#include <string>

#include <windows.h>

#include "exceptions/library_loader.hpp"

namespace burda
{
class library_loader
{
public:
    explicit library_loader(const std::string & library_path)
    {
        handle = ::LoadLibraryA(library_path.c_str());

        if (!handle)
        {
            throw exceptions::library_load_failed{ handle };
        }
    }

    virtual ~library_loader()
    {
        if (handle != nullptr)
        {
            ::FreeLibrary(handle);
            handle = nullptr;
        }
    }

    template<typename T>
    struct type_parser
    {
    };

    template<typename Ret, typename ... Args>
    struct type_parser<Ret(Args...)> {
        static std::function<Ret(Args...)> create_function(const FARPROC procedure_address) {
            return std::function<Ret(Args...)>(reinterpret_cast<Ret (__cdecl *)(Args...)>(procedure_address));
        }
    };

    template<typename function_type>
    std::function<function_type> get_procedure(const std::string & procedure_name)
    {
        std::function<function_type> procedure = nullptr;

        if (libraryHandle != nullptr)
        {
            const FARPROC procedure_address = ::GetProcAddress(libraryHandle, procedure_name.c_str());

            if (procedure_address == nullptr)
            {
                throw exceptions::library_loader::procedure_not_exists{ procedure_name };
            }
            else
            {
                procedure = type_parser<function_type>::create_function(procedure_address);
            }
        }

        return procedure;
}

private:
    HMODULE handle = nullptr;
};
}
}
