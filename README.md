![Version](https://img.shields.io/badge/version-1.2.1-green.svg)
[![License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/karel-burda/function-loader.svg?branch=develop)](https://travis-ci.org/karel-burda/function-loader)
[![Codecov Status](https://codecov.io/gh/karel-burda/function-loader/branch/develop/graph/badge.svg)](https://codecov.io/gh/karel-burda/function-loader/branch/develop)

# Important
This project contains git sub-modules that are needed for building tests.

If you just want to use the implementation, you can clone without sub-modules. In case you want to build the tests, be sure to clone the repository
with `--recurse-submodules` or `--recursive` on older versions of git. Alternatively, you can clone without sub-modules and initialize these later.

# Introduction
`function_loader` is a header-only library that can find free functions in a shared library and provides `std::function<T>` wrapper around the found function.

Essentially represents a wrapper around the calls `LoadLibrary`, `GetProcedure` and `FreeLibrary` system calls on Windows and `dlopen`, `dlsym` and `dlclose` on POSIXes.

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

add_subdirectory(<path-to-function-loader>)
# Example: add_subdirectory(function-loader ${CMAKE_BINARY_DIR}/function-loader)

# Query of package version
message(STATUS "Current version of function-loader is: ${function-loader_VERSION}")

add_library(burda::function-loader ALIAS function-loader)

# This will import search paths, compile definitions and other dependencies of the function-loader as well
target_link_libraries("my-project" function-loader)
# Or with private visibility: target_link_libraries("my-project" PRIVATE function-loader)

```

### B) Generate separately

Generation phase on the function-loader is run separately, that means that you run:
```cmake
cmake <path-to-function-loader>
# Example: cmake -Bbuild/function-loader -Hfunction-loader in the root of your project 
```

This will create automatically generated package configuration file `function-loader-config.cmake` that contains exported target and all important information.

Then you can do this in your CMakeLists.txt:

```cmake
add_executable("my-project" main.cpp)

find_package(function-loader CONFIG PATHS <path-to-binary-dir-of-function-loader>)
# Alternatively assuming that the "function-loader_DIR" variable is set: find_package(function-loader CONFIG)

# You can also query (or force specific version during the previous "find_package()" call)
message(STATUS "Found version of function-loader is: ${function-loader_VERSION}")

# This will import search paths, compile definitions and other dependencies of the function-loader as well
target_link_libraries("my-project" burda::function-loader)
# Or with public visibility: target_link_libraries("my-project" PUBLIC burda::function-loader)

```

## 2. Manual Way
Not recommended.

Make sure that the `include` directory is in the search paths.

You also have to set C++11 standard and potentially other settings as well (e.g. linking `libdl` on POSIXes).

# Example
For full use cases, see [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

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

# Unit Tests
Tests require sub-modules [cmake-helpers](https://github.com/karel-burda/cmake-helpers) and [test-utils](https://github.com/karel-burda/test-utils).

For building tests, run CMake in the source directory [tests/unit](tests/unit):

```cmake
cmake -Bbuild -H.
cmake -Bbuild/submodules/test-utils -Hsubmodules/test-utils
# You can also add coverage by appending "-DCOVERAGE:BOOL=ON"
cmake -Bbuild/tests/unit -Htests/unit
      -Dfunction-loader_DIR:PATH=$(pwd)/build
      -Dtest-utils_DIR:PATH=$(pwd)/build/submodules/test-utils
cmake --build build/tests/unit
# This runs target "run-all-tests-verbose" that will also run the tests with timeout, etc.:
cmake --build build/tests/unit --target run-all-tests-verbose
```

This is the example of running tests in the debug mode.

For more info, see [.travis.yml](.travis.yml).

# Continuous Integration
Continuous Integration is now being run Linux, OS X and Windows on Travis: https://travis-ci.org/karel-burda/cpp-utils.

Compilers are set-up to treat warnings as errors and with pedantic warning level.
Targets are built in one stage with debug symbols with code coverage measure and in release mode with debug symbols in the second one.

Valgrind is being run on the example as well.

The project is using these stages:
* `function-loader, example, tests -- linux, debug, cppcheck, coverage, g++, 64-bit`
* `function-loader, example, tests -- osx, release with debug info, valgrind, clang++, 64-bit`
* `function-loader, example, tests -- windows, release, msvc, 32-bit`

Project uses [codecov.io](https://codecov.io/gh/karel-burda/function-loader) for code coverage summary.
