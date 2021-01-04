#pragma once
#include "JobSystem.h"
#include "Timer.h"


namespace Trickster {
	class Engine;
	class Window;
	class AudioPlayer;
	class Application
	{
	public:
		TRICKSTER_API Application();
		TRICKSTER_API virtual ~Application();
		TRICKSTER_API static Application* Get();
		TRICKSTER_API virtual void Start();
		TRICKSTER_API virtual void OnUpdate(float a_DeltaTime) = 0;
		TRICKSTER_API virtual void OnStart() = 0;
		TRICKSTER_API virtual void OnRender() = 0;
		TRICKSTER_API virtual void OnPause(bool isPaused) = 0;
		TRICKSTER_API void Draw();
		TRICKSTER_API bool Update();
		TRICKSTER_API std::shared_ptr<Engine> GetEngine() const;
		TRICKSTER_API std::shared_ptr<Window> GetWindow() const;
		TRICKSTER_API std::shared_ptr<JobSystem> GetJobSystem() const;
		TRICKSTER_API virtual bool IsTickBased() = 0;
		TRICKSTER_API bool Paused() const;
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
