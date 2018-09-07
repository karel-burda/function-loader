#pragma once

#include <exception>
#include <functional>

#include <windows.h>

#include "function_extractor/exceptions.hpp"
#include "function_extractor/private/library_loader.base.hpp"

namespace burda
{
namespace function_extractor
{
class library_loader : public library_loader_base
{
public:
    explicit library_loader(const std::string & library_path)
        : library_loader_base(library_path)
    {
        handle = ::LoadLibraryExA(library_path.c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES);

        if (!handle)
        {
            throw exceptions::library_load_failed{ library_path };
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

        if (handle != nullptr)
        {
            const FARPROC procedure_address = ::GetProcAddress(handle, procedure_name.c_str());

            if (procedure_address == nullptr)
            {
                throw exceptions::function_does_not_exist{ procedure_name };
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
