cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tests LANGUAGES CXX)

message(STATUS "Building unit tests")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/cmake-helpers/cmake-helpers/cpp_coverage.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/cmake-helpers/cmake-helpers/cpp_gtest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../submodules/cmake-helpers/cmake-helpers/cpp_warnings.cmake)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                   PRIVATE
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/helpers.hpp

                       ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions_test.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/function_loader_test.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/library_loader_test.cpp)

find_package(test-utils CONFIG REQUIRED)
message(STATUS "Found version of test-utils is: ${test-utils_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE burda::test-utils)

burda_cmake_helpers_cpp_gtest_bootstrap_and_link(${PROJECT_NAME} "release-1.8.1" "Release" PRIVATE)

if (COVERAGE)
    message(STATUS "Building unit tests with code coverage")
    burda_cmake_helpers_cpp_coverage_add_build_options(${PROJECT_NAME} PRIVATE)
endif()

enable_testing()

add_test(NAME all-unit-tests
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
         COMMAND $<TARGET_FILE:${PROJECT_NAME}> --gtest_color=yes --gtest_shuffle)

add_custom_target(run-all-tests-verbose
                  COMMAND ${CMAKE_CTEST_COMMAND} -V -C ${CMAKE_BUILD_TYPE} --build-run-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --timeout 300
                  DEPENDS ${PROJECT_NAME})

burda_cmake_helpers_cpp_warnings_add_pedantic_level(${PROJECT_NAME} PRIVATE)
burda_cmake_helpers_cpp_warnings_suppress(${PROJECT_NAME} "keyword-macro" PRIVATE)
