#pragma once 

#include "KeyCodes.hpp"
#include "Event.hpp"

namespace Dvimana {
	class KeyboardKeyPressEvent : public Event {

		public:
			KeyboardKeyPressEvent(KeyCodes key) : m_Key(key) {}
			virtual ~KeyboardKeyPressEvent() = default;

			KeyCodes GetKeyCode() const { return m_Key; }
			EVENT_CLASS_TYPE(EventType::KeyboardKeyPress);
			EVENT_CLASS_CATEGORY(EventCategory::Keyboard);

		private:
			KeyCodes m_Key{ 0 };
	};

	class KeyboardKeyReleaseEvent : public Event {

		public:
			KeyboardKeyReleaseEvent(KeyCodes key) : m_Key(key) {}
			virtual ~KeyboardKeyReleaseEvent() = default;

			KeyCodes GetKeyCode() const { return m_Key; }
			EVENT_CLASS_TYPE(EventType::KeyboardKeyRelease);
			EVENT_CLASS_CATEGORY(EventCategory::Keyboard);

		private:
			KeyCodes m_Key{ 0 };
	};

	class KeyboardKeyRepeateEvent : public Event {

		public:
			KeyboardKeyRepeateEvent(KeyCodes key) : m_Key(key) {}
			virtual ~KeyboardKeyRepeateEvent() = default;

			KeyCodes GetKeyCode() const { return m_Key; }
			EVENT_CLASS_TYPE(EventType::KeyboardKeyRepeate);
			EVENT_CLASS_CATEGORY(EventCategory::Keyboard);

		private:
			KeyCodes m_Key{ 0 };
	};

	class KeyboardCharInputEvent : public Event {
		
		public:
			KeyboardCharInputEvent(uint32_t codepoint) : m_Codepoint(codepoint) {}
			virtual ~KeyboardCharInputEvent() = default;

			uint32_t GetCodepoint() const { return m_Codepoint; }
			EVENT_CLASS_TYPE(EventType::KeyboardCharInput);
			EVENT_CLASS_CATEGORY(EventCategory::Keyboard);

		private:
			uint32_t m_Codepoint{ 0 };
	};
}
