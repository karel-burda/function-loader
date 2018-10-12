#pragma once

#include <functional>

#include "function_loader/detail/function_loader_base.hpp"
#include "function_loader/detail/library_loader.hpp"

namespace burda
{
namespace function_loader
{
/// RAI wrapper that searches shared library for functions at run-time and binds these to the std::function type
/// Loads shared library handle in the construction and frees upon destruction
class function_loader : public detail::function_loader_base
{
public:
    /// Loads library handle as part of the construction
    /// @param library_path might be either absolute or relative to the working directory
    /// @see "LoadLibrary" from the Windows API or "dlopen" on POSIXes for explanation of the path argument
    /// @see "library_loder"
    /// @throws library_load_failed
    explicit function_loader(const std::string & library_path)
        : library{ library_path }
    {
    }

    /// Constructs std::function that binds to the shared library function
    /// @tparam is the type specifier of the std::function, e.g. "void(int, float)" for the "std::function<void(int, float)>"
    /// @returns std::function that binds to the found function in a shared library
    /// @throws function_does_not_exist
    template<typename function_type>
    std::function<function_type> get_function(const std::string & procedure_name) const
    {
        void * procedure_address = get_function_address(library.get_handle(), procedure_name.c_str());

        if (procedure_address == nullptr)
        {
            throw exceptions::function_does_not_exist{ procedure_name };
        }

        return reinterpret_cast<function_type*>(procedure_address);
    }

private:
    detail::library_loader library;
};
}
}
