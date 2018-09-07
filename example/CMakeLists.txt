cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example VERSION 0.0.1 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME} src/main.cpp)

# TODO: use find package instead
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

_add_pedantic_warning_level(${PROJECT_NAME})

# TODO: build also the demo library
if (MSVC)
    configure_file("resources/demo_library.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE} COPYONLY)
else()
    configure_file("resources/demo_library.dll" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
endif()
