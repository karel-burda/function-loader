#include <gtest/gtest.h>

#include <function_loader/exceptions.hpp>
#include <test_utils/make_all_members_public.hpp>
#include <function_loader/function_loader.hpp>
#include <test_utils/static_class_assertions.hpp>
#include <test_utils/test_utils.hpp>

#include "helpers.hpp"

namespace
{
namespace exceptions = burda::function_loader::exceptions;
namespace test_utils = burda::test_utils;
namespace testing = burda::function_loader::testing;

using function_loader = burda::function_loader::function_loader;

TEST(function_loader, static_assertions)
{
    test_utils::assert_default_constructibility<function_loader, false>();
    test_utils::assert_copy_constructibility<function_loader, false>();
    test_utils::assert_move_constructibility<function_loader, true>();
}

TEST(function_loader, construction_destruction)
{
    test_utils::assert_construction_and_destruction<function_loader>(testing::get_demo_library_file_path());
    EXPECT_THROW(function_loader{ "foo" }, exceptions::library_load_failed);
    EXPECT_NO_THROW(function_loader{ "./subdirectory/another/demo-library.dll" });
}

TEST(function_loader, move_operations)
{
    function_loader first_loader{ testing::get_demo_library_file_path() };

    ASSERT_NE(first_loader.library.get_handle(), nullptr);

    const auto second_loader = std::move(first_loader);

    ASSERT_EQ(first_loader.library.get_handle(), nullptr);
    ASSERT_NE(second_loader.library.get_handle(), nullptr);

    EXPECT_THROW(first_loader.get_function<void()>("function_with_no_params")(), exceptions::library_handle_invalid);
    EXPECT_NO_THROW(second_loader.get_function<void()>("function_with_no_params")());
}

TEST(function_loader, default_values)
{
    function_loader loader{ testing::get_demo_library_file_path() };
    EXPECT_NE(&loader.library, nullptr);
}

TEST(function_loader, get_function)
{
    function_loader loader{ testing::get_demo_library_file_path() };

    {
        const auto func_void_no_params = loader.get_function<void()>("function_with_no_params");

        EXPECT_NO_THROW(func_void_no_params());
    }

    {
        const auto func_with_return_value_and_params = loader.get_function<int(float, const char *)>("function_with_return_value_and_params");
        int returnedValue = 0;
        EXPECT_NO_THROW(returnedValue = func_with_return_value_and_params(2.3f, "foo-bar-baz"));
        EXPECT_EQ(returnedValue, 999);
    }

    {
        EXPECT_THROW(loader.get_function<int(float, const char *)>("function_that_does_not_exists"), exceptions::function_does_not_exist);
    }
}

TEST(function_loader, exceptions)
{
    // what() called on the exception should not be empty
    try
    {
        function_loader loader{ testing::get_demo_library_file_path() };
        loader.get_function<void()>("foo-bar-baz");
    }
    catch (const exceptions::function_does_not_exist & error)
    {
        EXPECT_NE(error.what(), "");
    }
    catch (...)
    {
        FAIL() << "Correct exception not thrown";
    }
}
}
