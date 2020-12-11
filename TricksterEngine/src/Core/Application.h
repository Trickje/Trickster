#pragma once
#include "Jobsystem.h"


namespace Trickster {
	class Engine;
	class Window;
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
		virtual bool IsTickBased() = 0;
	private:
		static Application* m_Application;
		std::shared_ptr<Engine> m_Engine;
	protected:
		std::shared_ptr<Window> m_Window;
		Timer m_Timer;
		//The multi-threading object ONLY ONE IS ALLOWED
		std::unique_ptr<Jobsystem> m_Jobsystem;
		float TickTime = 0.6f;
		float CurrentTickTime;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
