cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example VERSION 0.0.1 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                PUBLIC
                    src/main.cpp
                    src/main.cpp.in)

find_package(function-extractor REQUIRED CONFIG PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME} PUBLIC ${function-extractor_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${function-extractor_LIBRARIES})

_add_pedantic_warning_level(${PROJECT_NAME})

include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET default)
add_library(demo-library SHARED)
generate_export_header(demo-library EXPORT_FILE_NAME "export.hpp")
FILE(COPY 
    ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
    DESTINATION 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_sources(demo-library
                PUBLIC
                    src/demo-library.hpp
                    src/demo-library.cpp
                    src/export.hpp)

add_dependencies(${PROJECT_NAME} demo-library)

if (WIN32)
    set(DEMO_LIBRARY_PATH "demo-library.dll")
else()
    set(DEMO_LIBRARY_PATH "demo-library.so")
endif()
configure_file(src/main.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp @ONLY)
