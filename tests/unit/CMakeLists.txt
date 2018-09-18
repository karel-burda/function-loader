cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tests VERSION 0.0.1 LANGUAGES CXX)

_print_status_message("building with unit tests")
_print_project_version()

include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/gtest.cmake")
include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/threads.cmake")

_gtest_download_and_build("gtest-build" "Release" "gtest-src" "0957cce368316577aae5ddfffcb67f24621d69e7")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_sources(
    ${PROJECT_NAME}
        PUBLIC
            src/exceptions_test.cpp)

add_dependencies(${PROJECT_NAME} gtest-build)

_gtest_resolve_build_options(${PROJECT_NAME})

find_package(function-extractor REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${function-extractor_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${function-extractor_LIBRARIES})

find_package(test-utils REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${test-utils_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${test-utils_LIBRARIES})

if (COVERAGE)
    _print_status_message("building unit tests with code coverage")

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(
            ${PROJECT_NAME}
                PRIVATE
                    -fprofile-arcs
                    -ftest-coverage
                    -fcoverage-mapping
                    -fprofile-instr-generate)

        set_target_properties(
            ${PROJECT_NAME}
                PROPERTIES
                    LINK_FLAGS "-fprofile-arcs -ftest-coverage")
    endif()
endif()

enable_testing()

add_test(NAME all-unit-tests
         COMMAND $<TARGET_FILE:${PROJECT_NAME}> --gtest_color=yes --gtest_shuffle)

add_custom_target(run-all-tests-verbose
                    COMMAND ${CMAKE_CTEST_COMMAND} -V -C ${CMAKE_BUILD_TYPE} --timeout 300
                    DEPENDS ${PROJECT_NAME})

_add_pedantic_warning_level(${PROJECT_NAME})
_supress_cxx_compiler_warning(${PROJECT_NAME} "keyword-macro")
