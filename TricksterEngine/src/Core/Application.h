#pragma once
#include "JobSystem.h"


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
		virtual void OnPause(bool isPaused) = 0;
		
		void Draw();
		bool Update();
		std::shared_ptr<Engine> GetEngine() const;
		std::shared_ptr<Window> GetWindow() const;
		std::shared_ptr<JobSystem> GetJobSystem() const;
		virtual bool IsTickBased() = 0;
		bool Paused() const;
		const std::string TexturePath = "Resources/Textures/";
		const std::string ModelPath = "Resources/Models/";
		const std::string SoundPath = "Resources/Sounds/";
		const std::string ShaderPath = "Resources/Shaders/";
	private:
		static Application* m_Application;
		std::shared_ptr<Engine> m_Engine;
	protected:
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<AudioPlayer> m_AudioPlayer;
		Timer m_Timer;
		//The multi-threading object ONLY ONE IS ALLOWED
		std::shared_ptr<JobSystem> m_JobSystem;
		float TickTime = 0.6f;
		float CurrentTickTime;
		bool m_Paused;
		
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
