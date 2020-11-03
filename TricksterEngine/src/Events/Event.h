#pragma once
#include <complex>
#include <string>
#include <functional>
#include <list>

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
		void ExecuteAndClear(_Types... Args)
		{
			
			Execute(Args ...);
			m_Listeners.clear();
			if(!m_Listeners.empty())
			{
				LOG_ERROR("Failed to remove all subscribers from TickOnce!");
			};
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

	
	
}