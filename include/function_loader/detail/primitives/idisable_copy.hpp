#pragma once

/// Origin: https://github.com/karel-burda/cpp-utils
/// Version : 1.0.0

namespace burda
{
namespace cpp_utils
{
/// Helper class that enables default construction and disables copy operations
struct idisable_copy
{
    idisable_copy() = default;

    idisable_copy(const idisable_copy &) = delete;
    idisable_copy & operator=(const idisable_copy &) = delete;

    idisable_copy & operator=(idisable_copy &&) = default;
    idisable_copy(idisable_copy &&) = default;
};
}
}
