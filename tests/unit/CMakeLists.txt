cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tests LANGUAGES CXX)

_print_status_message("building with unit tests")
_print_project_version()

include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/code-coverage.cmake")
include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/gtest.cmake")
include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/threads.cmake")

_gtest_download_and_build("gtest-build" "Release" "gtest-src" "0957cce368316577aae5ddfffcb67f24621d69e7")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_sources(
    ${PROJECT_NAME}
        PUBLIC
            src/exceptions_test.cpp
            src/library_loader_test.cpp
            src/function_loader_test.cpp

        PRIVATE
            include/helpers.hpp)

add_dependencies(${PROJECT_NAME} gtest-build)

_gtest_resolve_build_options(${PROJECT_NAME})

find_package(function-loader REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${function-loader_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${function-loader_LIBRARIES})

find_package(test-utils REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${test-utils_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${test-utils_LIBRARIES})

if (COVERAGE)
    _print_status_message("building unit tests with code coverage")

    _coverage_add_build_options(${PROJECT_NAME})
endif()

enable_testing()

# We deliberately copy the testing library into a subdirectories in order to test whether loading of library from more subdirectories work
get_target_property(_demo-library_prefix demo-library PREFIX)
get_target_property(_demo-library_suffix demo-library SUFFIX)
set(_demo-library_location ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(_demo-library_filename "${_demo-library_prefix}demo-library${_demo-library_suffix}")
if (MSVC)
    string(APPEND _demo-library_location "/${CMAKE_BUILD_TYPE}")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${_demo-library_location}/${_demo-library_filename}
                       ${_demo-library_location}/subdirectory/another/${_demo-library_filename})

# create invalid (empty) file that will be used in the tests to verify, whether the implementation correctly refuses to load this file
#file(WRITE)

add_test(NAME all-unit-tests
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
         COMMAND $<TARGET_FILE:${PROJECT_NAME}> --gtest_color=yes --gtest_shuffle)

add_custom_target(run-all-tests-verbose
                    COMMAND ${CMAKE_CTEST_COMMAND} -V -C ${CMAKE_BUILD_TYPE} --build-run-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --timeout 300
                    DEPENDS ${PROJECT_NAME})

_add_pedantic_warning_level(${PROJECT_NAME})
_supress_cxx_compiler_warning(${PROJECT_NAME} "keyword-macro")
_supress_cxx_compiler_warning(${PROJECT_NAME} "maybe-uninitialized")
