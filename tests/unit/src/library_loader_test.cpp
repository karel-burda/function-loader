#include <gtest/gtest.h>

#include <function_extractor/exceptions.hpp>
#include <function_extractor/detail/library_loader.hpp>

#include <test_utils/static_class_assertions.hpp>
#include <test_utils/test_utils.hpp>

namespace
{
namespace function_extractor = burda::function_extractor;
namespace test_utils = burda::test_utils;

using library_loader = function_extractor::detail::library_loader;

TEST(library_loader_construction_destruction, static_assertions)
{
    test_utils::assert_default_constructibility<library_loader, false>();
}

TEST(library_loader_construction_destruction, basic)
{
    test_utils::assert_construction_and_destruction<library_loader>("./demo-library.dll");
    EXPECT_THROW(library_loader{ "foo" }, function_extractor::exceptions::library_load_failed);
    EXPECT_NO_THROW(library_loader{ "./demo-library.dll" });
}
}
