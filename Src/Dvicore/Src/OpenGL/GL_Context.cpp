#include "GL_Context.hpp"
#include "Assert.hpp"

namespace DviCore 
{
    OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Window(window){}

    bool OpenGLContext::MakeContext()
    {
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        GLenum status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if(status == 0)
            DVIMANA_ASSERT(false, "Failed to initialize GLAD!");

        DVI_CORE_INFO("GLEW initialized");
        return true;
    }

    void OpenGLContext::SwapBuffers()
    {
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwSwapBuffers(m_Window);
    }

    void OpenGLContext::SwapInterval(uint32_t interval)
    {
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwSwapInterval(interval);
    }
}