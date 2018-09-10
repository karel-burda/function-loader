#pragma once

#ifdef _WIN32
#include "function_extractor/private/library_loader_impl_windows.hpp"
#elif __linux__
#include "function_extractor/private/library_loader_impl_unix.hpp"
#else
#error Platform not implemented
#endif
