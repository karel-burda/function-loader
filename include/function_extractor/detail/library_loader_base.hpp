#pragma once

#ifdef _WIN32
#include "function_extractor/detail/library_loader_base_windows.hpp"
#elif __linux__
#include "function_extractor/detail/library_loader_base_unix.hpp"
#else
#error Platform not implemented
#endif
