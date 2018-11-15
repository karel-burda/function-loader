![Version](https://img.shields.io/badge/version-1.2.1-green.svg)
[![License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/karel-burda/function-loader.svg?branch=master)](https://travis-ci.org/karel-burda/function-loader)
[![Coverage Status](https://coveralls.io/repos/github/karel-burda/function-loader/badge.svg?branch=master)](https://coveralls.io/github/karel-burda/function-loader?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/fd08a5e184a945208324fd7a415428ad)](https://app.codacy.com/app/karel-burda/function-loader?utm_source=github.com&utm_medium=referral&utm_content=karel-burda/function-loader&utm_campaign=Badge_Grade_Dashboard)

# Important
This project contains git sub-modules that are needed for building example and tests.

If you just want to use the implementation, you can clone **without** sub-modules. In case you want to build example, tests, or use CMake, please, be sure to clone the repository
with `--recurse-submodules` or `--recursive` on older versions of git. Alternatively, you can clone without sub-modules and initialize these later.

# Introduction
`function_loader` is a header-only library that can find free functions in a shared library and provide and `std::function<T>` wrapper around the found function.

Essentially provides wrapper around the calls `LoadLibrary`, `GetProcedure` and `FreeLibrary` system calls on Windows and `dlopen`, `dlsym` and `dlclose` on POSIXes.

`function_loader` class supports move semantics and disables copy operations; implementation is in C++11.

These exceptions might be thrown:
* `library_load_failed`
* `library_handle_invalid`
* `function_does_not_exist`

Exceptions provide additional information about the reason using `what()`,  see [exceptions.hpp](include/function_loader/exceptions.hpp) for more info.

# Usage
In order to use the `function_loader`, it's the `include` directory that matters. Just make sure that the header search path is pointing to the [include](include) directory located in the root directory.

Implementation resides in the `burda::function_loader` namespace, so it might be useful to do something like `namespace fe = burda::function_loader;` in your project.

There are basically these options when it comes to build system integration:

## 1. CMake Way
Recommended option.

There are essentially these ways of how to use this package depending on your preferences our build architecture:

### A) Generate directly

Call `add_subdirectory(...)` directly in your CMakeLists.txt:

```cmake
add_executable("my-project" main.cpp)

add_subdirectory(<path-to-test-utils>)
# Example: add_subdirectory(test-utils ${CMAKE_BINARY_DIR}/test-utils)

# Query of package version
message(STATUS "Current version of test-utils is: ${test-utils_VERSION}")

add_library(burda::test-utils ALIAS test-utils)

# This will import search paths, compile definitions and other dependencies of the test-utils as well
target_link_libraries("my-project" test-utils)
# Or with private visibility: target_link_libraries("my-project" PRIVATE test-utils)

```

### B) Generate separately

Generation phase on the test-utils is run separately, that means that you run:
```cmake
cmake <path-to-test-utils>
# Example: cmake -Bbuild/test-utils -Htest-utils in the root of your project 
```

This will create automatically generated package configuration file `test-utils-config.cmake` that contains exported target and all important information.

Then you can do this in your CMakeLists.txt:

```cmake
add_executable("my-project" main.cpp)

find_package(test-utils CONFIG PATHS <path-to-binary-dir-of-test-utils>)
# Alternatively assuming that the "test-utils_DIR" variable is set: find_package(test-utils CONFIG)

# You can also query (or force specific version during the previous "find_package()" call)
message(STATUS "Found version of test-utils is: ${test-utils_VERSION}")

# This will import search paths, compile definitions and other dependencies of the test-utils as well
target_link_libraries("my-project" burda::test-utils)
# Or with public visibility: target_link_libraries("my-project" PUBLIC burda::test-utils)

```

## 2. Manual Way
Not recommended.

Make sure that the `include` directory is in the search paths.

You also have to set C++11 standard and potentially other settings as well (e.g. linking `libdl` on POSIXes).

### Example
```cpp
#include <iostream>

#include <function_loader/exceptions.hpp>
#include <function_loader/function_loader.hpp>

namespace function_loader = burda::function_loader;

try
{
    // or load DLL on Windows, or .dylib on OS X
    function_loader::function_loader loader{ "./shared-library.so" };
    // function_loader supports move semantics, so we can safely do e.g. "const auto other = std::move(loader)"

    // get procedures at runtime from the shared library
    // see "demo-library.hpp" and "demo-library.cpp" in the "demo-library" directory
    const auto func_simple = loader.get_function<void()>("foo");
    const auto func_more_complex = loader.get_function<int(float, const char *)>("bar");

    // don't have to check for call-ability, otherwise the "function_does_not_exist" would be thrown
    func_simple();
    std::clog << "func_more_complex returned " << func_more_complex(99.0, "foo");

    // if the "loader" object went out of scope in here, it would free all resources and unload
    // the library handle, but for demo purposes, we'll move the instance
    const auto another_loader = std::move(loader);
    // do whatever actions you like with the "another_loader"

    // the "another_loader" goes out of scope
}
catch (const function_loader::exceptions::library_load_failed & error)
{
    // handle exception, error.what() contains information about the error code from the OS
    // library load failed upon construction of the function_loader
}
catch (const function_loader::exceptions::library_handle_invalid & error)
{
    // happens when "get_function" called on the function_loader with invalid library handle
    // (may happen after the object was moved)
}
catch (const function_loader::exceptions::function_does_not_exist & error)
{
    // given function not found in the library, might be caused by incorrect signature,
    // or function is not exported (visible) from outside
}
```

Where this is the header of the `shared-library.(so|dll|dylib)`:
```cpp
extern "C"
{
/// LIBRARY_EXPORT is defined elsewhere, but we just need the symbols to be visible from outside
/// the shared libary (e.g. using "__declspec(dllexport)" or "__attribute__((visibility("default")))" on the GCC).
/// When using function_loader, we don't need to import any symbols (e.g. "__declspec(dllimport)"),
/// because there's no static linking.

LIBRARY_EXPORT void foo();
LIBRARY_EXPORT int bar(float number, const char * str);
}
```

For full use cases, see [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

# Build Process
Library itself is just header-only, so no need for additional linking, just `libdl` needs to be linked to the final executable on POSIXes. See section [Usage](#Usage) for more info.

In order to build the usage example ([main.cpp](example/src/main.cpp)) run the cmake in the top-level directory like this:

`cmake .`

I personally prefer to specify a separate build directory explicitly:

`cmake -Bbuild -H.`

You can of course specify ordinary cmake options like build type (debug, release with debug info, ...), used generator, etc.

# Unit Tests
Tests require sub-module [cmake-helpers](https://github.com/karel-burda/cmake-helpers) and [test-utils](https://github.com/karel-burda/test-utils).

For building tests, run CMake in the source directory [tests/unit](tests/unit):

```cmake
cmake -Bbuild -H.
# You can also add coverage by appending "-DCOVERAGE:BOOL=ON"
cmake -Bbuild/tests/unit -Htests/unit -Dcpp-utils_DIR:PATH=$(pwd)/build -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo
cmake --build build/tests/unit --config RelWithDebInfo
# This runs target "run-all-tests-verbose" that will also run the tests with timeout, etc.:
cmake --build build/tests/unit --target run-all-tests-verbose --config RelWithDebInfo
# Or you can execute tests manually
```

For more info, see [.travis.yml](.travis.yml).

# Continuous Integration
Continuous Integration is now being run Linux (with GCC 6.x) and OS X on Travis: https://travis-ci.org/karel-burda/cpp-utils.

Compilers are set-up to treat warnings as errors and with pedantic warning level.
Targets are built debug symbols with code coverage measure and release with debug symbols).

The project is using thse stages:
* `cpp-utils, tests -- linux, debug, gcc, cppcheck, coverage`
* `cpp-utils, tests -- osx, release with debug info, clang`

Project uses [codecov.io](https://codecov.io/gh/karel-burda/cpp-utils) for code coverage summary.
