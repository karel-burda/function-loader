cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                PUBLIC
                    src/main.cpp)

find_package(function-extractor REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${function-extractor_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${function-extractor_LIBRARIES})

_add_pedantic_warning_level(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} demo-library)
