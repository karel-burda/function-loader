cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(demo-library VERSION 0.8.0 LANGUAGES CXX)

_print_status_message("building demo library")
_print_project_version()

include(GenerateExportHeader)

add_library(${PROJECT_NAME} SHARED)

_add_pedantic_warning_level(${PROJECT_NAME})

set(CMAKE_CXX_VISIBILITY_PRESET default)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "export.hpp")
FILE(COPY 
    ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
    DESTINATION 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${PROJECT_NAME}
                   PUBLIC
                       src/demo-library.hpp
                       src/demo-library.cpp
                       src/export.hpp)

# Unfortunately, we cannot use SUFFIX as empty string property on MSVC, because the LoadLibrary method from WinAPI
# checks whether the file name has some suffix at all. Now using the same "dll" suffix only in Unixes
set_target_properties(${PROJECT_NAME}
                          PROPERTIES
                              PREFIX ""
                              SUFFIX ".dll")

