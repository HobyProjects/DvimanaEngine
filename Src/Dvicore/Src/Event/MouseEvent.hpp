#pragma once

#include "Event.hpp"

namespace Dvimana {

    class MouseButtonPressEvent : public Event {
        public:
            MouseButtonPressEvent(int32_t button) : m_button(button) {}
            virtual ~MouseButtonPressEvent() = default;

            int32_t GetButton() const { return m_button; }

            EVENT_CLASS_TYPE(EventType::MouseButtonPress);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);

        private:
            int32_t m_button{0};
    };

    class MouseButtonReleaseEvent : public Event {
        public:
            MouseButtonReleaseEvent(int32_t button) : m_button(button) {}
            virtual ~MouseButtonReleaseEvent() = default;

            int32_t GetButton() const { return m_button; }

            EVENT_CLASS_TYPE(EventType::MouseButtonRelease);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);
        
        private:
            int32_t m_button{0};
    };

    class MouseWheelEvent : public Event {
        public:
            MouseWheelEvent(double xoffset, double yoffset) : m_xoffset(xoffset), m_yoffset(yoffset) {}
            virtual ~MouseWheelEvent() = default;

            double GetXOffset() const { return m_xoffset; }
            double GetYOffset() const { return m_yoffset; }

            EVENT_CLASS_TYPE(EventType::MouseWheelScroll);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);

        private:
            double m_xoffset{0}, m_yoffset{0};
    };

    class CursorPosChangeEvent : public Event {
        public:
            CursorPosChangeEvent(double xpos, double ypos) : m_xpos(xpos), m_ypos(ypos) {}
            virtual ~CursorPosChangeEvent() = default;

            double GetXPos() const { return m_xpos; }
            double GetYPos() const { return m_ypos; }

            EVENT_CLASS_TYPE(EventType::MouseCursorPosChange);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);
        
        private:
            double m_xpos{0}, m_ypos{0};
    };

    class CursorEnterEvent : public Event {
        public:
            CursorEnterEvent() = default;
            virtual ~CursorEnterEvent() = default;

            EVENT_CLASS_TYPE(EventType::MouseCursorEnter);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);
    };

    class CursorLeaveEvent : public Event {
        public:
            CursorLeaveEvent() = default;
            virtual ~CursorLeaveEvent() = default;

            EVENT_CLASS_TYPE(EventType::MouseCursorLeave);
            EVENT_CLASS_CATEGORY(EventCategory::Mouse);
    };

}