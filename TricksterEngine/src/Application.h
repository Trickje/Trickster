#pragma once
#include <memory>


#include "Timer.h"
#include "Engine.h"

namespace Trickster {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Start();
		virtual void OnUpdate(float a_DeltaTime) = 0;
		virtual void OnStart() = 0;
		virtual void OnRender() = 0;
		virtual void OnEventListen() = 0;
		virtual void OnEventExecute() = 0;
		bool Update();
		std::shared_ptr<Engine> GetEngine() const;
	private:
		std::shared_ptr<Engine> m_Engine;
		Timer m_Timer;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
