#pragma once

#include <string>
#include <memory>
#include "GL_Context.hpp"

namespace DviCore 
{
    enum class WindowState 
    {
        Minimized,
        Maximized,
        Normal
    };

    struct WindowSpecification 
    {
        std::string Title{std::string()};
        uint32_t Width{0};
        uint32_t Height{0};
        bool IsVSyncEnabled{true};
        bool IsActive{false};
        bool IsFocused{false};
        WindowState State{WindowState::Normal};

        int32_t RedColorBits{8};
        int32_t GreenColorBits{8};
        int32_t BlueColorBits{8};
        int32_t AlphaColorBits{8};
        int32_t DepthColorBits{24};
        int32_t StencilColorBits{8};

        int32_t RefreshRate{60};
        int32_t FixedWidth{0};
        int32_t FixedHeight{0};
        uint32_t MinWidth{1280};
        uint32_t MinHeight{720};

        uint32_t PosX{0};
        uint32_t PosY{0};

        int32_t FramebufferWidth{0};
        int32_t FramebufferHeight{0};
    };

    class Window 
    {
        public:
            Window(const std::string& title = "Dvimana Engine");
            ~Window();

            GLFWwindow* GetNativeWindow() const { return m_Window; }
            WindowSpecification& GetWindowSpecification() { return m_Specification; }
            std::shared_ptr<OpenGLContext> GetOpenGLContext() const { return m_Context; }
            void SwapBuffers();

        private:
            GLFWwindow* m_Window{nullptr};
            WindowSpecification m_Specification{}; 
            std::shared_ptr<OpenGLContext> m_Context{nullptr}; 
    };
}