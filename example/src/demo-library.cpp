#include "demo-library.hpp"

#include <iostream>
#include <string>

namespace burda
{
namespace function_extractor
{
namespace example
{
void function_with_no_params()
{
    std::cout << "Executing 'function_with_no_params'" << std::endl;
}

int function_with_return_value_and_params(float number, const char * str)
{
    std::cout << "Executing 'function_with_return_value_and_params'" << std::endl;

    std::cout << "number=" << std::to_string(number) << ", str=" << (str ? str : "(nullptr)") << std::endl;

    return 999;
}
}
}
}
