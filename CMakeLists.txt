cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(function-extractor VERSION 0.1.1 LANGUAGES CXX)

include("cmake-helpers/cmake-helpers/messages.cmake")
include("cmake-helpers/cmake-helpers/standard-settings.cmake")
include("cmake-helpers/cmake-helpers/warnings.cmake")

_print_project_version()

_set_standards("11" "14")
_set_default_build_type("Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(EXAMPLE "Build example usage" ON)
option(UNIT-TESTS "Build unit tests" OFF)
if (EXAMPLE)
    add_subdirectory(example)
endif()

if (UNIT-TESTS)
    enable_testing()
    #add_subdirectory(tests/unit)
endif()

set(HEADERS
    include/function_extractor/function_extractor.hpp
    include/function_extractor/library_loader.hpp

    include/function_extractor/exceptions.hpp

    include/function_extractor/private/library_loader_interface.hpp
    include/function_extractor/private/library_loader_impl.hpp

    $<$<PLATFORM_ID:Windows>:include/function_extractor/private/library_loader_impl_windows.hpp>
    $<$<PLATFORM_ID:Unix>:include/function_extractor/private/library_loader_impl_posix.hpp>)

set(OTHER
    README.md
    .travis.yml)

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${OTHER})
