macro(_burda_function_loader_demo_library_add_post_build_steps _target)
    get_target_property(_demo-library_prefix demo-library PREFIX)
    get_target_property(_demo-library_suffix demo-library SUFFIX)
    set(_demo-library_location ${CMAKE_CURRENT_BINARY_DIR}/demo-library)
    set(_demo-library_filename ${_demo-library_prefix}demo-library${_demo-library_suffix})
    if (MSVC)
        string(APPEND _demo-library_location /${CMAKE_BUILD_TYPE})
    endif()

    add_custom_command(TARGET ${_target}
            POST_BUILD
                COMMAND
                    ${CMAKE_COMMAND} -E copy
                        ${_demo-library_location}/${_demo-library_filename}
                        ${CMAKE_CURRENT_BINARY_DIR}/${_demo-library_filename}

                # We deliberately copy the testing library into a subdirectories in order to test whether loading of library from nested dirs work
                COMMAND
                    ${CMAKE_COMMAND} -E copy
                        ${_demo-library_location}/${_demo-library_filename}
                        ${CMAKE_CURRENT_BINARY_DIR}/subdirectory/another/${_demo-library_filename})
endmacro()
