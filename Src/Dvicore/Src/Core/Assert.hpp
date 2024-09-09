#pragma once

#include <filesystem>
#include "Log.hpp"
#include "Base.hpp"

#ifdef DVIMANA_ASSERTS_ENABLED
#define DVIMANA_ASSERT(expression, ...)                                                                                                         \
    if (!(expression))                                                                                                                          \
    {                                                                                                                                           \
        DVI_CORE_CRITICAL("Assertion failed ({0}|{1}): {2}", std::filesystem::path(__FILE__).filename().string(), __LINE__, __VA_ARGS__);       \
        DVIMANA_DEBUGBREAK();                                                                                                                   \
    }
#else
#define DVIMANA_ASSERT(expression, ...)
#endif