#include <gtest/gtest.h>

#include <function_loader/exceptions.hpp>
#include <test_utils/static_class_assertions.hpp>

namespace
{
namespace exceptions = burda::function_loader::exceptions;
namespace test_utils = burda::test_utils;

TEST(exceptions, static_assertions)
{
    test_utils::assert_default_constructibility<exceptions::function_does_not_exist, false>();
    test_utils::assert_default_constructibility<exceptions::library_load_failed, false>();
}
}
