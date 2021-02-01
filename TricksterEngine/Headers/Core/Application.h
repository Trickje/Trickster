#pragma once
#include "JobSystem.h"
#include "Timer.h"
/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */

namespace Trickster {
	class Engine;
	class Window;
	class AudioPlayer;
	class RenderAPI;
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
		std::shared_ptr<RenderAPI> m_RenderAPI;
		float m_FrameCount;
		float m_TimePassedSinceLastFPSCount;
	protected:
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<AudioPlayer> m_AudioPlayer;
		Timer m_Timer;
		//The multi-threading object ONLY ONE IS ALLOWED
		std::shared_ptr<JobSystem> m_JobSystem;
		float TickTime = 0.6f;
		float CurrentTickTime;
		bool m_Paused;
		float m_AverageFPS;
		
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
