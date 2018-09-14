#include <gtest/gtest.h>

#include <function_extractor/exceptions.hpp>

#include <test_utils/static_class_assertions.hpp>

namespace
{
namespace function_extractor = burda::function_extractor;
namespace test_utils = burda::test_utils;

TEST(exceptions_construction_destruction, static_assertions)
{
    test_utils::assert_default_constructibility<function_extractor::exceptions::function_does_not_exist, false>();
}
}
