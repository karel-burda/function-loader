#pragma once

#ifdef _WIN32
#include "function_extractor/private/library_loader.windows.hpp"
#else
#error Platform is not supported
#endif
