#include "demo-library.hpp"

#include <iostream>
#include <string>

namespace burda
{
namespace function_loader
{
namespace example
{
void function_with_no_params()
{
    std::clog << "Executing 'function_with_no_params'" << std::endl;
}

int function_with_return_value_and_params(float number, const char * str)
{
    std::clog << "Executing 'function_with_return_value_and_params', parameters: number="
              << std::to_string(number) << ", str=" << (str ? str : "(nullptr)") << std::endl;

    return 999;
}
}
}
}
