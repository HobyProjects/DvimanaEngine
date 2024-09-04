#pragma once

#include "Event.hpp"

namespace Dvimana {

    class WindowResizeEvent : public Event {

        public:
            WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}
            virtual ~WindowResizeEvent() = default;

            uint32_t GetWidth() const { return m_Width; }
            uint32_t GetHeight() const { return m_Height; }
            EVENT_CLASS_TYPE(EventType::WindowResize);
            EVENT_CLASS_CATEGORY(EventCategory::Application);

        private:
            uint32_t m_Width{ 0 };
            uint32_t m_Height{ 0 };
    };

    class WindowFrameResizeEvent : public Event {

        public:
            WindowFrameResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}
            virtual ~WindowFrameResizeEvent() = default;

            int32_t GetWidth() const { return m_Width; }
            int32_t GetHeight() const { return m_Height; }
            EVENT_CLASS_TYPE(EventType::WindowFrameResize);
            EVENT_CLASS_CATEGORY(EventCategory::Application);

        private:
            uint32_t m_Width{ 0 };
            uint32_t m_Height{ 0 };
    };

    class WindowCloseEvent : public Event {

        public:
            WindowCloseEvent() = default;
            virtual ~WindowCloseEvent() = default;

            EVENT_CLASS_TYPE(EventType::WindowClose);
            EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class WindowPosChangeEvent : public Event {
        
        public:
            WindowPosChangeEvent(int32_t x, int32_t y) : m_X(x), m_Y(y) {}
            virtual ~WindowPosChangeEvent() = default;

            int32_t GetX() const { return m_X; }
            int32_t GetY() const { return m_Y; }
            EVENT_CLASS_TYPE(EventType::WindowPosChange);
            EVENT_CLASS_CATEGORY(EventCategory::Application);

        private:
            int32_t m_X{ 0 };
            int32_t m_Y{ 0 };
    };

    class WindowMinimizeEvent : public Event {

        public:
            WindowMinimizeEvent() = default;
            virtual ~WindowMinimizeEvent() = default;

            EVENT_CLASS_TYPE(EventType::WindowMinimized);
            EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class WindowMaximizeEvent : public Event {

        public:
            WindowMaximizeEvent() = default;
            virtual ~WindowMaximizeEvent() = default;

            EVENT_CLASS_TYPE(EventType::WindowMaximized);
            EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class WindowFocusLostEvent : public Event {

        public:
            WindowFocusLostEvent() = default;
            virtual ~WindowFocusLostEvent() = default;

            EVENT_CLASS_TYPE(EventType::WindowFocusLost);
            EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class WindowFocusGainedEvent : public Event {

        public:
            WindowFocusGainedEvent() = default;
            virtual ~WindowFocusGainedEvent() = default;

            EVENT_CLASS_TYPE(EventType::WindowFocusGain);
            EVENT_CLASS_CATEGORY(EventCategory::Application);
    };



}