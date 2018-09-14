#include <gtest/gtest.h>

#include <function_extractor/exceptions.hpp>
#include <function_extractor/library_loader.hpp>

#include <test_utils/test_utils.hpp>

namespace
{
namespace function_extractor = burda::function_extractor;
namespace test_utils = burda::test_utils;

TEST(exceptions_construction_destruction, static_assertions)
{
    test_utils::assert_construction_and_destruction<function_extractor::library_loader>("demo-library");
}

TEST(exceptions_construction_destruction, basic)
{
    EXPECT_THROW(function_extractor::library_loader{ "foo" }, function_extractor::exceptions::library_load_failed);
}
}
