cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GenPkgConfig)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "7")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

project(function-loader VERSION ${CPACK_PACKAGE_VERSION} LANGUAGES CXX)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set("PROJECT_DESCRIPTION" "Header-only library that can execute functions in a shared library and provides wrappers around these")
set("PROJECT_HOMEPAGE_URL" "https://github.com/karel-burda/function-loader")
set(CPACK_PACKAGE_VENDOR "Karel Burda")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")


add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
                   INTERFACE
                       $<BUILD_INTERFACE:
                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/function_loader.hpp
                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/exceptions.hpp

                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/function_loader_base.hpp
                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/function_loader_platform_specific.hpp
                       $<$<PLATFORM_ID:Windows>:
                           ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/function_loader_platform_specific_windows.hpp>
                       $<$<PLATFORM_ID:Unix>:
                           ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/function_loader_platform_specific_unix.hpp>

                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/library_loader.hpp
                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/library_loader_base.hpp
                       ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/library_loader_platform_specific.hpp
                       $<$<PLATFORM_ID:Windows>:
                           ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/library_loader_platform_specific_windows.hpp>
                       $<$<PLATFORM_ID:Unix>:
                           ${CMAKE_CURRENT_LIST_DIR}/include/function_loader/detail/library_loader_platform_specific_unix.hpp>
                       >)

target_include_directories(${PROJECT_NAME}
                                INTERFACE
                                    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if (CMAKE_DL_LIBS)
    message(STATUS "Will add ${CMAKE_DL_LIBS}")
    set_target_properties(${PROJECT_NAME}
                              PROPERTIES
                                  INTERFACE_LINK_LIBRARIES "${CMAKE_DL_LIBS}")
endif()

target_compile_features(${PROJECT_NAME}
                            INTERFACE
                                cxx_std_11)

install(TARGETS ${PROJECT_NAME}
        EXPORT _targets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

set(CONFIG_FILE_NAME_WITHOUT_EXT "${PROJECT_NAME}-config")

install(EXPORT _targets
        FILE ${CONFIG_FILE_NAME_WITHOUT_EXT}.cmake
        NAMESPACE "burda::"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(EXPORT _targets
       NAMESPACE burda::
       FILE "${PROJECT_NAME}-config.cmake")

include(CMakePackageConfigHelpers)

set(CMAKE_CONFIG_FILE_BASENAME "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME_WITHOUT_EXT}")
set(CMAKE_CONFIG_VERSION_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}-version.cmake")
set(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}.cmake")

if(${CMAKE_VERSION} VERSION_GREATER "3.14")
    set(OPTIONAL_ARCH_INDEPENDENT "ARCH_INDEPENDENT")
endif()

write_basic_package_version_file("${CMAKE_CONFIG_VERSION_FILE_NAME}"
                                 COMPATIBILITY ExactVersion
                                 ${OPTIONAL_ARCH_INDEPENDENT})

install(FILES "${CMAKE_CONFIG_VERSION_FILE_NAME}"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")


set(PackagingTemplatesDir "${CMAKE_CURRENT_SOURCE_DIR}/packaging")

string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)
set(PKG_CONFIG_NAME "${PROJECT_NAME}")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/function_loader"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

configure_pkg_config_file("${PROJECT_NAME}"
    NAME "${PROJECT_NAME}"
    VERSION "${PROJECT_VERSION}"
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    URL "${PROJECT_HOMEPAGE_URL}"
    INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}"
    INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")

set("CPACK_DEBIAN_PACKAGE_NAME" "lib${PROJECT_NAME}-dev")
set("CPACK_DEBIAN_PACKAGE_DEPENDS" "libc6-dev")
set("CPACK_DEBIAN_PACKAGE_SUGGESTS" "cmake, pkg-config, pkg-conf")

set("CPACK_RPM_PACKAGE_NAME" "lib${PROJECT_NAME}-devel")
set("CPACK_RPM_PACKAGE_SUGGESTS" "${CPACK_DEBIAN_PACKAGE_SUGGESTS}")

set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_NSIS_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

include(CPack)
