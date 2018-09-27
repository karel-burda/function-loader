cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(function-extractor VERSION 0.9.0 LANGUAGES CXX)

include("cmake-helpers/cmake-helpers/messages.cmake")
include("cmake-helpers/cmake-helpers/standard-settings.cmake")
include("cmake-helpers/cmake-helpers/warnings.cmake")

option(EXAMPLE "Build example usage" ON)
option(UNIT-TESTS "Build unit tests" OFF)

_print_project_version()

_set_standards("11" "14")
_set_default_build_type("Debug")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(HEADERS
    include/function_extractor/function_loader.hpp
    include/function_extractor/exceptions.hpp

    include/function_extractor/detail/function_loader_base.hpp
    $<$<PLATFORM_ID:Windows>:include/function_extractor/detail/function_loader_base_windows.hpp>
    $<$<PLATFORM_ID:Unix>:include/function_extractor/detail/function_loader_base_unix.hpp>

    include/function_extractor/detail/library_loader.hpp
    include/function_extractor/detail/library_loader_base.hpp
    $<$<PLATFORM_ID:Windows>:include/function_extractor/detail/library_loader_base_windows.hpp>
    $<$<PLATFORM_ID:Unix>:include/function_extractor/detail/library_loader_base_unix.hpp>)

set(OTHER
    README.md
    .travis.yml
    ${PROJECT_NAME}-config.cmake.in)

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${OTHER})

configure_file(${PROJECT_NAME}-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)

if (EXAMPLE OR UNIT-TESTS)
    add_subdirectory(demo-library)

    if (EXAMPLE)
        add_subdirectory(example)
    endif()
    
    if (UNIT-TESTS)
        add_subdirectory(test-utils)

        add_subdirectory(tests/unit)
    endif()
endif()
