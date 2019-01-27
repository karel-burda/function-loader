cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(demo-library LANGUAGES CXX)

message(STATUS "Building demo library")

include(${CMAKE_CURRENT_SOURCE_DIR}/../submodules/cmake-helpers/cmake-helpers/cpp_warnings.cmake)

add_library(${PROJECT_NAME} SHARED)

include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET default)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "export.hpp")
FILE(COPY ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_sources(${PROJECT_NAME}
                   PUBLIC
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/demo-library.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/demo-library.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/export.hpp)

# Unfortunately, we cannot use SUFFIX as empty string property on MSVC, because the LoadLibrary method from WinAPI
# checks whether the file name has some suffix at all. For simplicity, use the same suffix also on POSIXEs
set_target_properties(${PROJECT_NAME}
                          PROPERTIES
                              PREFIX ""
                              SUFFIX ".dll")

burda_cmake_helpers_cpp_warnings_add_pedantic_level(${PROJECT_NAME} PRIVATE CXX)
