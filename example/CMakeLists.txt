cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example VERSION 0.0.1 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME} src/main.cpp src/main.cpp.in)

# TODO: use find package instead
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

_add_pedantic_warning_level(${PROJECT_NAME})

# TODO: build also the demo library
# get target property library name
if (WIN32)
    set(DEMO_LIBRARY_PATH "demo-library.dll")
else()
    set(DEMO_LIBRARY_PATH "demo-library.so")
endif()
configure_file(src/main.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp @ONLY)
