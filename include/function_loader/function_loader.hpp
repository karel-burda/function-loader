#pragma once

#include <functional>

#include "function_loader/detail/function_loader_platform_specific.hpp"

namespace burda
{
namespace function_loader
{
/// RAI wrapper that searches shared library for functions at run-time and binds these to the std::function type
/// Loads shared library handle in the construction and frees upon destruction
class function_loader : public detail::function_loader_platform_specific
{
public:
    /// Loads library handle as part of the construction
    /// Usage:library_loader{"./foo/bar/library.(so|dll|dylib)"}
    /// @param library_path might be either absolute or relative to the working directory
    /// @see "LoadLibrary" from the Windows API or "dlopen" on POSIXes for explanation of the path argument
    /// @see "library_lodader_base"
    /// @throws library_load_failed
    using function_loader_platform_specific::function_loader_platform_specific;

    /// Constructs std::function that binds to the shared library function
    /// @tparam is the type specifier of the std::function, e.g. "void(int, float)" for the "std::function<void(int, float)>"
    /// @returns std::function that binds to the found function in a shared library
    /// @throws function_does_not_exist
    template<typename function_type>
    std::function<function_type> get_function(const std::string & procedure_name) const
    {
        void * procedure_address = get_function_address(procedure_name);

        if (procedure_address == nullptr)
        {
            throw exceptions::function_does_not_exist{ procedure_name };
        }

        return reinterpret_cast<function_type*>(procedure_address);
    }
};
}
}
