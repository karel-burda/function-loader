#pragma once

#ifdef _WIN32
#include "function_loader/detail/function_loader_platform_specific_windows.hpp"
#elif defined(__unix__) || defined(__APPLE__)
#include "function_loader/detail/function_loader_platform_specific_unix.hpp"
#else
#error Platform not implemented
#endif
