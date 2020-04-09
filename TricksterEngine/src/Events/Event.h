#pragma once
#include <string>
#include <functional>

namespace Trickster
{

	enum class EventType
	{
		None = 0,
		//All of the events
	};

	enum EventCategory
	{
		None = 0,
		//Categories
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){ return EventType::##type;}\
								virtual EventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory a_Category)
		{
			return GetCategoryFlags() & a_Category;
		}
	protected:
		bool m_Handled = false;
	};

	
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& a_Event)
		: m_Event(a_Event)
		{
			
		}
		template <typename T>
		bool Dispatch(EventFn<T> func)
		{
			if(m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
			}
		}
	private:
		Event& m_Event;
	};

	
	
}