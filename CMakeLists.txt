cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(function-dll-extractor VERSION 0.0.1 LANGUAGES CXX)

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
    add_subdirectory(tests/unit)
endif()

set(HEADERS
    include/function-dll-extractor/library-loader.hpp)

set(OTHER
    README.md
    .travis.yml)

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${OTHER})
