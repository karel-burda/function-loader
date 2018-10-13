![Version](https://img.shields.io/badge/version-1.2.0-green.svg)
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
On POSIXes, the `libdl` has to be linked to the final executable.

You can use the provided CMake package configuration at [function-loader-config.cmake.in](function-loader-config.cmake.in).

Implementation resides in the `burda::function_loader` namespace, so it might be useful to do something like `namespace fe = burda::function_loader;` in your project.

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

    // if the "loader" object went out of scope in here, it would free all resources and unload the library handle,
    // but for demo purposes, we'll move the instance
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
/// LIBRARY_EXPORT is defined elsewhere, but we just need the symbols to be visible from outside the shared libary
/// (e.g. using "__declspec(dllexport)" or "__attribute__((visibility("default")))" on the GCC).
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
For building tests, run cmake with the option `UNIT-TESTS=ON`:

`cmake -Bbuild -H. -DUNIT-TESTS:BOOL=ON`

The project is using the `gtest` that is automatically downloaded, "cmaked" and built in its build step
(the fixed stable revision of the `gtest` is used).

Then, you can run the default test target (e.g. `make test` or `RUN_TESTS` in the Visual Studio)
or the custom target `run-all-tests-verbose` (which is recommended and used in the Continuous Integration).

The target `run-all-tests-verbose` uses the `ctest` for executing the tests binary and has built-in timeout feature (useful because of dead-locks for example).

If you want to debug tests and implementation, run the target `tests` target manually (ideally in the Debug mode).

It is also possible to turn off build of the example, and build just the tests:

`cmake -Bbuild -H. -DEXAMPLE:BOOL=OFF -DUNIT-TESTS:BOOL=ON`

# Continuous Integration
Continuous Integration is now being run Linux (with GCC 5.x) on Travis: https://travis-ci.org/karel-burda/function-loader.

Compilers are set-up to treat warnings as errors and with pedantic warning level. Targets are built in a release mode with debug symbols (because of the [valgrind](http://valgrind.org) and code coverage measure).

The project is using just one stage (because of the overhead of spawning other stages)
* `example and tests (C++14)` -- cppcheck, build (linux, gcc5), valgrind, tests

Project uses [coveralls.io](https://coveralls.io/github/karel-burda/function-loader) for code coverage summary and [codacy](https://app.codacy.com/app/karel-burda/function-loader/dashboard) for the coding style and additional static analysis.
