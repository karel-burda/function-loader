#pragma once

#include "export.hpp"

namespace burda
{
namespace function_extractor
{
namespace example
{
extern "C"
{
DEMO_LIBRARY_EXPORT void function_with_no_params();
DEMO_LIBRARY_EXPORT int function_with_return_value_and_params(float number, const char * str);
}
}
}
}
