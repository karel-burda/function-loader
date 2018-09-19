#pragma once

#include <functional>

#include "function_extractor/library_loader.hpp"

namespace burda
{
namespace function_extractor
{
class function_loader
{
public:
    explicit function_loader(const std::string & library_path)
        : library{ library_path }
    {

    }

    template<typename T>
    struct type_parser
    {
    };

    template<typename Ret, typename... Args>
    struct type_parser<Ret(Args...)> {
        static std::function<Ret(Args...)> create_function(const FARPROC procedure_address) {
            return std::function<Ret(Args...)>(reinterpret_cast<Ret(__cdecl *)(Args...)>(procedure_address));
        }
    };

    template<typename function_type>
    std::function<function_type> get_procedure(const std::string & procedure_name)
    {
        std::function<function_type> procedure = nullptr;

        const FARPROC procedure_address = GetProcAddress(library.get_handle(), procedure_name.c_str());

        if (procedure_address == nullptr)
        {
            throw exceptions::function_does_not_exist{ procedure_name };
        }
        else
        {
            procedure = type_parser<function_type>::create_function(procedure_address);
        }

        return procedure;
    }

private:
    library_loader library;
};
}
}
