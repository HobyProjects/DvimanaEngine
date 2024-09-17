#pragma once 

#include <string>
#include <functional>
#include <type_traits>

#include "Log.hpp"

namespace DviCore 
{
	enum class EventCategory 
	{
		Application,
		Keyboard,
		Mouse,
		Unknown
	};

	enum class EventType 
	{
		WindowResize,
		WindowClose,
		WindowPosChange,
		WindowMaximized,
		WindowMinimized,
		WindowFocusGain,
		WindowFocusLost,
		WindowFrameResize,

		KeyboardKeyPress,
		KeyboardKeyRelease,
		KeyboardKeyRepeate,
		KeyboardCharInput,

		MouseButtonPress,
		MouseButtonRelease,
		MouseWheelScroll,
		MouseCursorPosChange,
		MouseCursorEnter,
		MouseCursorLeave,
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; } \
								  virtual EventType GetEventType() const override { return GetStaticType(); } \
								  virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual EventCategory GetCategory() const override { return category; }

	class Event 
	{
		public:
			Event() = default;
			virtual ~Event() = default;

			virtual EventType GetEventType() const = 0;
			virtual EventCategory GetCategory() const = 0;
			virtual const char* GetName() const = 0;
			virtual std::string ToString() const { return GetName(); }

			bool CategoryEquals(EventCategory category) const 
			{
				return GetCategory() == category;
			}

		public:
			bool Handled{ false };
	};

	class EventHandler 
	{	
		public:
			EventHandler(Event& event) : 
				m_Event(event) {}

			template<typename T, typename Function>
			bool Dispatch(const Function& function) 
			{
				if (m_Event.GetEventType() == T::GetStaticType()) 
				{
					m_Event.Handled |= function(static_cast<T&>(m_Event));
					return true;
				}
				
				return false;
			}

		private:
			Event& m_Event;
	};

	using EventCallback = std::function<void(Event&)>;
}

#define DVI_CALLBACK(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }