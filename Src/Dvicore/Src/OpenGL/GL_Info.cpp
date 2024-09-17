#include "GL_Info.hpp"

namespace DviCore 
{
    std::string OpenGLInfo::GetVendor()
    {
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        return std::string(vendor);
    }

    std::string OpenGLInfo::GetRenderer()
    {
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        return std::string(renderer);
    }

    std::string OpenGLInfo::GetVersion()
    {
        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        return std::string(version);
    }

    std::string OpenGLInfo::GetGLSLVersion()
    {
        const char* version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        char GL_MAJOR = version[0];
        char GL_MINOR = version[2];

        std::stringstream ss;
        ss << "#version " << GL_MAJOR << GL_MINOR << 0 << " core";
        return ss.str();
    }
}