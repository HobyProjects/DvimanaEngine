#pragma once

#include "WindowEvent.hpp"
#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"
#include "Window.hpp"

namespace DviCore {
    class EventReceiver {
        private:
            EventReceiver() = default;
            ~EventReceiver() = default;

        public:
            static void PollEvents();
            static void SetWindowCallback(const std::shared_ptr<Window>& window, const EventCallback& callback);
    };
}