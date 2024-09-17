#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DviCore 
{
    class OpenGLContext 
    {
        public:
            OpenGLContext() = default;
            OpenGLContext(GLFWwindow* window);
            ~OpenGLContext() = default;

            bool MakeContext();
            void SwapBuffers();
            void SwapInterval(uint32_t interval);
        
        private:
            GLFWwindow* m_Window{nullptr};
    };
}