#include "Window.hpp"

namespace Dvimana {
    Window::Window(const std::string & title){
        Log::Init();
        if(!glfwInit()) {
            DVI_CORE_CRITICAL("Failed to initialize GLFW");
            return;
        }

        DVI_CORE_INFO("GLFW initialized");
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if(mode != nullptr){
            m_Specification.Width               = mode->width;
            m_Specification.Height              = mode->height;
            m_Specification.FixedWidth          = mode->width;
            m_Specification.FixedHeight         = mode->height;
            m_Specification.MinWidth            = 1024;
            m_Specification.MinHeight           = 720;
            m_Specification.RedColorBits        = mode->redBits;
            m_Specification.GreenColorBits      = mode->greenBits;
            m_Specification.BlueColorBits       = mode->blueBits;
            m_Specification.AlphaColorBits      = 8;
            m_Specification.RefreshRate         = mode->refreshRate;
        }
        else {
            DVI_CORE_WARN("Failed to get primary monitor video mode");;
            m_Specification.Width           = 1280;
            m_Specification.Height          = 720;
            m_Specification.FixedWidth      = 0;
            m_Specification.FixedHeight     = 0;
            m_Specification.MinWidth        = 1024;
            m_Specification.MinHeight       = 720;
            m_Specification.RedColorBits    = 8;
            m_Specification.GreenColorBits  = 8;
            m_Specification.BlueColorBits   = 8;
            m_Specification.AlphaColorBits  = 8;
            m_Specification.RefreshRate     = 60;
        }

        m_Specification.Title = title;
        
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        glfwWindowHint(GLFW_RED_BITS, m_Specification.RedColorBits);
        glfwWindowHint(GLFW_GREEN_BITS, m_Specification.GreenColorBits);
        glfwWindowHint(GLFW_BLUE_BITS, m_Specification.BlueColorBits);
        glfwWindowHint(GLFW_ALPHA_BITS, m_Specification.AlphaColorBits);
        glfwWindowHint(GLFW_REFRESH_RATE, m_Specification.RefreshRate);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        m_Window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, title.c_str(), nullptr, nullptr);
        if(m_Window != nullptr){
            DVI_CORE_INFO("GLFW window created");
            glfwSetWindowSizeLimits(m_Window, m_Specification.MinWidth, m_Specification.MinHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
            glfwGetFramebufferSize(m_Window, &m_Specification.FramebufferWidth, &m_Specification.FramebufferHeight);

            m_Context = std::make_shared<OpenGLContext>(m_Window);
            m_Context->MakeContext();
            
            m_Specification.IsActive = true;
            m_Specification.IsFocused = glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
            m_Specification.IsVSyncEnabled = true;
            return;
        }

        DVI_CORE_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
    }

    Window::~Window(){
        if(m_Window != nullptr)
            glfwDestroyWindow(m_Window);

        glfwTerminate();
    }
}