#pragma once

#include <functional>

#include "function_extractor/detail/function_loader_base.hpp"
#include "function_extractor/detail/library_loader.hpp"

namespace burda
{
namespace function_extractor
{
class function_loader : public detail::function_loader_base
{
public:
    explicit function_loader(const std::string & library_path)
        : library{ library_path }
    {
    }

    template<typename function_type>
    std::function<function_type> get_procedure(const std::string & procedure_name)
    {
        std::function<function_type> procedure = nullptr;

        void * procedure_address = get_function_address(library.get_handle(), procedure_name.c_str());

        if (procedure_address == nullptr)
        {
            throw exceptions::function_does_not_exist{ procedure_name };
        }
        else
        {
            procedure = reinterpret_cast<function_type*>(procedure_address);
        }

        // todo: check for null (different signature etc.)

        return procedure;
    }

private:
    detail::library_loader library;
};
}
}
