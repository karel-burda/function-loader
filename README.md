![Version](https://img.shields.io/badge/version-0.1.1-green.svg)
[![License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/karel-burda/function-extractor.svg?branch=develop)](https://travis-ci.org/karel-burda/function-extractor)

# Important
**This project contains git sub-modules, please, be sure to clone the repository with `--recurse-submodules` or `--recursive` on older versions of git.**

# Introduction
`function_extractor` is a header-only library that can find free functions in a shared library and provide and `std::function<T>` wrapper around the found function.

Essentially a wrapper around the calls `LoadLibrary`, `GetProcedure` and `FreeLibrary` system calls on Windows and `dlopen`, `dlsym` and `dlclose` on POSIXes.

* `library_loader`

Implemented using C++11 for Windows and POSIX systems.

These exceptions might be thrown:
* ``

See [exceptions.h](include/function_extractor/exceptions.h) for more info.

# Usage
In order to use the `function_extractor`, it's the `include` directory that matters. Just make sure that the header search path is pointing to the [include](include) directory located in the root directory.
On POSIXes, the `libdl` has to be linked to the final executable.

Tou can use the provided package config at [function-extractor-config.cmake.in](function-extractor-config.cmake.in).

Implementation resides in the `burda::function_extractor` namespace, so it might be useful to do `namespace fe = burda::function_extractor;` in your project.

See also section [Requirements](#Requirements).

### Example
```cpp
// TODO
```
For full use cases, see [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

# Build Process
Library itself is just header-only, so no need for additional linking, just threading library might need to be linked to the final executable on most Linux standard library implementations. See section [Usage](#Usage) for more info.

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
Continuous Integration is now being run Linux (gcc 5.x and clang 5.x) on Travis: https://travis-ci.org/karel-burda/function-extractor.

Compilers are set-up to treat warnings as errors and with pedantic warning level. Targets are built in a release mode with debug symbols (because of the [valgrind](http://valgrind.org) and code coverage measure).

The project is using just one stage (because of the overhead of spawning other stages)
* `example and tests (C++14)` -- cppcheck, build (linux, gcc5), valgrind, tests

Project uses [coveralls.io](https://coveralls.io/github/karel-burda/function-extractor) for code coverage summary and [codacy](https://app.codacy.com/app/karel-burda/function-extractor/dashboard) for the coding style and additional static analysis.
