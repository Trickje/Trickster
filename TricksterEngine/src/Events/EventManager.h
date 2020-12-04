#pragma once
#include "Event.h"
#include "GameLoopEvents.h"
#include "InputEvents.h"
#include "JobEvents.h"
#include "WindowEvents.h"


namespace Trickster {
	class EventManager
	{
	public:
		static EventManager* GetInstance();
			/*
		 *Event<> = no parameters
		 *Event<float> = 1 parameter in float
		 *
		 * 
		 */
		void Initialize();

		GameLoopEvents GameLoopEvents;
		JobEvents JobEvents;
		InputEvents InputEvents;
		WindowEvents WindowEvents;
	private:
		static EventManager* instance;
		EventManager();
		~EventManager();
	};

}
