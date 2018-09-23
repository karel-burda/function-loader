#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

#include <gtest/gtest.h>

#include <function_extractor/exceptions.hpp>
#include <function_extractor/detail/library_loader.hpp>

#include <test_utils/static_class_assertions.hpp>
#include <test_utils/test_utils.hpp>

#include "helpers.hpp"

namespace
{
namespace function_extractor = burda::function_extractor;
namespace test_utils = burda::test_utils;
namespace testing = function_extractor::testing;

using library_loader = function_extractor::detail::library_loader;

TEST(library_loader_construction_destruction, static_assertions)
{
    test_utils::assert_default_constructibility<library_loader, false>();
    test_utils::assert_copy_constructibility<library_loader, false>();
    test_utils::assert_move_constructibility<library_loader, false>();
}

TEST(library_loader_construction_destruction, basic)
{
    test_utils::assert_construction_and_destruction<library_loader>(testing::get_demo_library_file_path());
    EXPECT_THROW(library_loader{ "foo" }, function_extractor::exceptions::library_load_failed);
    EXPECT_NO_THROW(library_loader{ testing::get_demo_library_file_path() });
    EXPECT_NO_THROW(library_loader{ "./subdirectory/another/demo-library.dll" });
    EXPECT_THROW(library_loader{ "./subdirectory/another/a/b/c/d/demo-library.dll" }, function_extractor::exceptions::library_load_failed);
}

/*
TEST_F(library_loader, default_values)
{
    // TODO
}
*/

TEST(library_loader_construction_destruction, resource_deallocation)
{
    library_loader shared_library{ testing::get_demo_library_file_path() };

    const auto handle = shared_library.get_handle();

    shared_library.~library_loader();

    // error is expected, because the handle is freed
#ifdef _WIN32
    EXPECT_EQ(FreeLibrary(handle), 0);
#elif __linux__
    EXPECT_NE(dlclose(handle), 0);
#endif
}
}
