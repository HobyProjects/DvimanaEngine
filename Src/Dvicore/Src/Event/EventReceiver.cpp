#include "EventReceiver.hpp"

namespace DviCore {

    static EventCallback s_EventCallback;
    static std::shared_ptr<Window> s_Window{nullptr};

    void EventReceiver::SetWindowCallback(const std::shared_ptr<Window>& window, const EventCallback& callback) 
    {
        s_EventCallback = callback;
        s_Window = window;

        glfwSetWindowCloseCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window) 
        {
            WindowCloseEvent windowClose;
            s_EventCallback(windowClose); 
        });

        glfwSetWindowSizeCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int width, int height) 
        {
            WindowResizeEvent windowResize(width, height);
            s_Window->GetWindowSpecification().Width = width;
            s_Window->GetWindowSpecification().Height = height;
            s_EventCallback(windowResize);
        });

        glfwSetWindowPosCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int x, int y) 
        {
            WindowPosChangeEvent windowPosChange(x, y);
            s_Window->GetWindowSpecification().PosX = x;
            s_Window->GetWindowSpecification().PosY = y;
            s_EventCallback(windowPosChange);
        });

        glfwSetWindowFocusCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int focused) 
        {
            if (focused) {
                WindowFocusGainedEvent windowFocusGain;
                s_Window->GetWindowSpecification().IsFocused = true;
                s_EventCallback(windowFocusGain);
            } 
            else {
                WindowFocusLostEvent windowFocusLost;
                s_Window->GetWindowSpecification().IsFocused = false;
                s_EventCallback(windowFocusLost);
            }
        });

        glfwSetWindowMaximizeCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int maximized) 
        {
            WindowMaximizeEvent windowMaximized;
            s_Window->GetWindowSpecification().State = WindowState::Maximized;
            s_EventCallback(windowMaximized);
        });

        glfwSetWindowIconifyCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int iconified) 
        {
            WindowMinimizeEvent windowMinimized;
            s_Window->GetWindowSpecification().State = WindowState::Minimized;
            s_EventCallback(windowMinimized);
        });

        glfwSetFramebufferSizeCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int width, int height) 
        {
            WindowFrameResizeEvent windowFrameResize(width, height);
            s_Window->GetWindowSpecification().FramebufferWidth = width;
            s_Window->GetWindowSpecification().FramebufferHeight = height;
            s_EventCallback(windowFrameResize);
        });

        glfwSetCursorEnterCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int entered) 
        {
            if (entered) {
                CursorEnterEvent cursorEnter;
                s_EventCallback(cursorEnter);
            } 
            else {
                CursorLeaveEvent cursorLeave;
                s_EventCallback(cursorLeave);
            }
        });

        glfwSetCursorPosCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, double x, double y) 
        {
            CursorPosChangeEvent cursorMove(x, y);
            s_EventCallback(cursorMove);
        });

        glfwSetScrollCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, double xoffset, double yoffset) 
        {
            MouseWheelEvent scroll(xoffset, yoffset);
            s_EventCallback(scroll);
        });

        glfwSetMouseButtonCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int button, int action, int mods) 
        {
            if(action == GLFW_PRESS) {
                MouseButtonPressEvent mouseButtonPress(button);
                s_EventCallback(mouseButtonPress);
            } 
            else if(action == GLFW_RELEASE) {
                MouseButtonReleaseEvent mouseButtonRelease(button);
                s_EventCallback(mouseButtonRelease);
            }
        });

        glfwSetKeyCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) 
        {
            if(action == GLFW_PRESS) {
                KeyboardKeyPressEvent keyPress(GetKeyCode(key));
                s_EventCallback(keyPress);
            } 
            else if(action == GLFW_RELEASE) {
                KeyboardKeyReleaseEvent keyRelease(GetKeyCode(key));
                s_EventCallback(keyRelease);
            }
            else if(action == GLFW_REPEAT) {
                KeyboardKeyRepeateEvent keyRepeat(GetKeyCode(key));
                s_EventCallback(keyRepeat);
            }
        });

        glfwSetCharCallback(s_Window->GetNativeWindow(), [](GLFWwindow* window, unsigned int codepoint) 
        {
            KeyboardCharInputEvent keyChar(codepoint);
            s_EventCallback(keyChar);
        });
    }

    void EventReceiver::PollEvents() 
    {
        glfwWaitEvents();
    }
}