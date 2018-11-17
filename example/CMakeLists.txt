cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example LANGUAGES CXX)

message(STATUS "Building example")

include(${CMAKE_CURRENT_SOURCE_DIR}/../submodules/cmake-helpers/cmake-helpers/cpp_warnings.cmake)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                   PRIVATE
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

find_package(function-loader CONFIG REQUIRED)
message(STATUS "Found version of function-loader is: ${function-loader_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE burda::function-loader)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../demo-library ${CMAKE_CURRENT_BINARY_DIR}/demo-library)
add_dependencies(${PROJECT_NAME} demo-library)

include(${CMAKE_CURRENT_SOURCE_DIR}/../demo-library/post_build_steps.cmake)
_burda_function_loader_demo_library_add_post_build_steps(${PROJECT_NAME})

burda_cmake_helpers_cpp_warnings_add_pedantic_level(${PROJECT_NAME})
