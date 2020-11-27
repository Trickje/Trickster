#pragma once
#include "Event.h"



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

		
		Event<> OnRender;
		Event<> OnRenderTransparent;
		Event<float> OnUpdate;
		Event<> OnStart;
		Event<> OnWindowClose;
		Event<int, int> OnWindowResize;
		Event<bool> OnWindowMinimize;
		Event<bool> OnWindowMaximize;
		Event<int> OnKeyPressed;
		Event<int> OnKeyReleased;
		Event<int, int> OnKeyRepeat;
		Event<int> OnMouseButtonPressed;
		Event<int> OnMouseButtonReleased;
		Event<float, float> OnMouseButtonScrolled;
		Event<float, float> OnMouseMoved;
		Event<> Tick;
		Event<> TickOnce;
		Event<> PreTick;
		Event<> PostTick;
	private:
		static EventManager* instance;
		EventManager();
		~EventManager();
	};

}