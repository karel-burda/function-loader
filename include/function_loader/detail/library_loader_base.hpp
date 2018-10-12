#pragma once

#include <string>

#include "function_loader/exceptions.hpp"
#include "function_loader/detail/cpp_utils/primitives/idisable_copy.hpp"

namespace burda
{
namespace function_loader
{
namespace detail
{
template <typename handle_type>
class library_loader_base : private burda::cpp_utils::primitives::idisable_copy
{
public:
    handle_type get_handle() const
    {
        return handle;
    }

protected:
    void throw_if_handle_is_invalid()
    {
        if (handle == nullptr)
        {
            throw exceptions::library_handle_invalid{};
        }
    }

    handle_type handle = nullptr;
};
}
}
}
