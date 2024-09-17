#pragma once

#include <string>
#include <sstream>
#include <glad/glad.h>

namespace DviCore 
{
    class OpenGLInfo 
    {
        public:
            static std::string GetVendor();
            static std::string GetRenderer();
            static std::string GetVersion();
            static std::string GetGLSLVersion();
    };
}
