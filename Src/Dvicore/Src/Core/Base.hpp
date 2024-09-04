#pragma once

#include "Platform.hpp"

#ifdef _DEBUG
#if defined(DVIMANA_PLATFORM_WINDOWS)
#define DVIMANA_DEBUGBREAK() __debugbreak()
#elif defined(DVIMANA_PLATFORM_LINUX)
	#include <signal.h>
	#define DVIMANA_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "Platform doesn't support debugbreak yet!"
#endif
#define DVIMANA_INSTRUMENTS_ENABLED
#define DVIMANA_ASSERTS_ENABLED
#define DVI_MACTOSTRING(x) #x
#else
#define DVIMANA_DEBUGBREAK()
#define DVI_MACTOSTRING(x) #x
#endif