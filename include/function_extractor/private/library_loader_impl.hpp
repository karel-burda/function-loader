#pragma once

#ifdef _WIN32
#include "function_extractor/private/library_loader_impl_windows.hpp"
#elif __posix
#include "function_extractor/private/library_loader_impl_posix.hpp"
#else
#error Platform not implemented
#endif
