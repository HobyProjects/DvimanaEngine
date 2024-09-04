#pragma once

#include "Window.hpp"
#include "KeyCodes.hpp"

namespace Dvimana {
    class InputHandler {
        public:
            static void TargetWindow(const std::shared_ptr<Window>& window);
            static bool KeyPressed(KeyCodes key);
            static bool MouseClicked(MouseButton button);
            static glm::vec2 GetCurrentMousePosition();

        private:
            InputHandler() = default;
            ~InputHandler() = default;
            InputHandler(InputHandler const &) = delete;
            InputHandler& operator=(InputHandler const &) = delete;
            InputHandler(InputHandler &&) = delete;
            InputHandler& operator=(InputHandler &&) = delete;
    };
}