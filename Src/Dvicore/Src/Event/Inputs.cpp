#include "Inputs.hpp"

namespace Dvimana {

    static std::shared_ptr<Window> s_TargetWindow{nullptr};

    void InputHandler::TargetWindow(const std::shared_ptr<Window>& window){
        s_TargetWindow = window;
    }

    bool InputHandler::KeyPressed(KeyCodes key){
        DVIMANA_ASSERT(s_TargetWindow != nullptr, "Target window not set");
        auto state = glfwGetKey(s_TargetWindow->GetNativeWindow(), static_cast<int>(key));
        return state == GLFW_PRESS;
    }

    bool InputHandler::MouseClicked(MouseButton button){
        DVIMANA_ASSERT(s_TargetWindow != nullptr, "Target window not set");
        auto state = glfwGetMouseButton(s_TargetWindow->GetNativeWindow(), static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 InputHandler::GetCurrentMousePosition(){
        DVIMANA_ASSERT(s_TargetWindow != nullptr, "Target window not set");
        double xpos, ypos;
        glfwGetCursorPos(s_TargetWindow->GetNativeWindow(), &xpos, &ypos);
        return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

