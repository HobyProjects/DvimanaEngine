#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define DVIMANA_PLATFORM_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!!!"
    #elif TARGET_OS_IPHONE == 1
        #error "IOS is not supported!!!"
    #elif TARGET_OS_MAC == 1
        #error "MacOS is not supported"
    #else
        #error "Unknown apple platform"
    #endif
#endif


#ifdef __ANDROID__
    #error "Android platform is not supported"
#endif

#ifdef __linux__
    #define DVIMANA_PLATFORM_LINUX
#endif