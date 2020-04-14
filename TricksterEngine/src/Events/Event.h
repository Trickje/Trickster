#pragma once
#include <complex>
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

	template<class... _Types>
	class Event
	{
		using Signature = void(_Types...);
		using EventFn = std::function<Signature>;
	public:
		static int GetArgNumber()
		{
			return sizeof...(_Types);
		}
	
		void AddListener(EventFn a_Listener)
		{
			m_Listeners.push_back(a_Listener);
		}
		void Execute(_Types... Args)
		{
			for(auto& F : m_Listeners)
			{
				F(std::forward<_Types>(Args)...);
			}
		}
		
		void RemoveListener(EventFn a_Listener)
		{
			for(auto& F : m_Listeners)
			{
				if(F->target<Signature>() == a_Listener.target<Signature>())
				{
					m_Listeners.erase(F);
				}
			}
		}

		
		EventType GetEventType() const;
		const char* GetName() const ;
		int GetCategoryFlags() const;
		std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory a_Category)
		{
			return GetCategoryFlags() & a_Category;
		}
	//protected:
	//	bool m_Handled = false;
	private:
		std::list<EventFn> m_Listeners;
	};

	/*
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

	*/
	
}