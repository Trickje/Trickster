#pragma once
#include <memory>


#include "Timer.h"
#include "Engine.h"
#include "Window.h"

namespace Trickster {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		static Application* Get();
		virtual void Start();
		virtual void OnUpdate(float a_DeltaTime) = 0;
		virtual void OnStart() = 0;
		virtual void OnRender() = 0;
		
		void Draw();
		bool Update();
		std::shared_ptr<Engine> GetEngine() const;
		std::shared_ptr<Window> GetWindow() const;
	private:
		static Application* m_Application;
		std::shared_ptr<Engine> m_Engine;
	protected:
		std::shared_ptr<Window> m_Window;
		Timer m_Timer;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
