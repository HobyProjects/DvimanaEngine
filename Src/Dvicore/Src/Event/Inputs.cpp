#include "Inputs.hpp"
#include "Assert.hpp"

namespace DviCore 
{
    static std::shared_ptr<Window> s_TargetWindow{nullptr};

    void InputHandler::TargetWindow(const std::shared_ptr<Window>& window)
    {
        s_TargetWindow = window;
    }

    bool InputHandler::KeyPressed(KeyCodes key)
    {
        if(!s_TargetWindow)
        {
            DVI_CORE_ERROR("Target window not set");
            return false;
        }

        auto state = glfwGetKey(s_TargetWindow->GetNativeWindow(), static_cast<int>(key));
        return state == KEY_PRESSED || state == KEY_REPEAT;
    }

    bool InputHandler::MouseClicked(MouseButton button)
    {
        if(!s_TargetWindow)
        {
            DVI_CORE_ERROR("Target window not set");
            return false;
        }

        auto state = glfwGetMouseButton(s_TargetWindow->GetNativeWindow(), static_cast<int>(button));
        return state == MOUSE_BUTTON_PRESSED;
    }

    glm::vec2 InputHandler::GetCurrentMousePosition()
    {
        if(!s_TargetWindow)
        {
            DVI_CORE_ERROR("Target window not set");
            return glm::vec2(0.0f);
        }
        
        double xpos, ypos;
        glfwGetCursorPos(s_TargetWindow->GetNativeWindow(), &xpos, &ypos);
        return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

