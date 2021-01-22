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
#include "pch.h"
#include "Core/Application.h"


#include "Core/AudioPlayer.h"
#include "Core/Engine.h"
#include "Events/EventManager.h"
#include "Core/Input.h"
#include "Rendering/Window.h"
#include "Rendering/ShaderManager.h"

#include <curl/curl.h>
#include <curl/mprintf.h>

#include "Core/Version.h"
#include "Rendering/Text/TextRenderer.h"
using namespace Trickster;

Application* Application::m_Application = nullptr;
Application::Application()
{
	LOG("Trickster Engine is running on version " + std::to_string(TRICKSTER_VERSION_MAJOR) + "." + std::to_string(TRICKSTER_VERSION_MINOR) + "." + std::to_string(TRICKSTER_VERSION_PATCH) + "." + std::to_string(TRICKSTER_VERSION_BUILD));
	m_Timer.Start();
	m_Engine = std::make_shared<Engine>();
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_JobSystem = std::make_shared<JobSystem>();
	m_AudioPlayer = std::make_shared<AudioPlayer>();
	EventManager::GetInstance()->GameLoopEvents.OnStart.AddListener(std::bind(&Application::OnStart, this));
	EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Application::OnRender, this));
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&Window::OnUpdate, m_Window.get()));
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&Application::OnUpdate, this, std::placeholders::_1));
	EventManager::GetInstance()->GameLoopEvents.OnStart.AddListener(std::bind(&ShaderManager::Initialize, ShaderManager::GetInstance()));
	m_Application = this;
	m_Paused = false;
	m_Window->CaptureMouse(true);
	m_AverageFPS = 0.f;
	m_TimePassedSinceLastFPSCount = 0.f;
}


Application::~Application()
{
//m_Engine implicitly deleted
	delete TextRenderer::Get();
}

Application* Application::Get()
{
	if(m_Application == nullptr)
	{
		LOG_ERROR("Application was not found.");
	}
	return m_Application;
}

void Application::Start()
{
	if (!m_Engine->Initialize())
	{
		LOG_ERROR("Failed to initialize the engine!");
	}
	if(!m_AudioPlayer->Initialize())
	{
		LOG_ERROR("Failed to initialize the Audio Player!");
	}
	CURL* hnd = curl_easy_init();
	delete hnd;
	hnd = NULL;
	std::srand(static_cast<unsigned int>(time(0)));
	EventManager::GetInstance()->GameLoopEvents.OnStart.Execute();
	float time_passed = m_Timer.GetSeconds();
	if (time_passed < 1.f) {
#ifdef DETAILED_CONSOLE
		LOG("[Application] Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
#endif
	}else
	{
#ifdef DETAILED_CONSOLE
		LOG_WARNING("[Application] Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
#endif
	}
	m_Timer.Reset();
	CurrentTickTime = 0.f;

}

void Application::Draw()
{
	m_Window->Draw();
}

bool Trickster::Application::Update()
{
	if(m_Paused)
	{
		m_Timer.Pause();
		if (Input::GetClick(Mouse::Left))
		{
			//UNPAUSE
			m_Paused = false;
			m_Window->CaptureMouse(true);
			OnPause(false);
		}
	}else
	{
		if (Input::GetKeyDown(Keys::ESC))
		{
			//PAUSE
			m_Paused = true;
			m_Window->CaptureMouse(false);
			OnPause(true);
			
		}
		m_Timer.Pause(false);
		float DeltaTime = m_Timer.Reset();
		m_AudioPlayer->Update(DeltaTime);
		//DELTA TIME calculations
		EventManager::GetInstance()->GameLoopEvents.OnUpdate.Execute(DeltaTime);
		CurrentTickTime += DeltaTime;
		while (CurrentTickTime >= TickTime)
		{
			CurrentTickTime -= TickTime;
			EventManager::GetInstance()->GameLoopEvents.PreTick.Execute();
			EventManager::GetInstance()->GameLoopEvents.Tick.Execute();
			EventManager::GetInstance()->GameLoopEvents.TickOnce.ExecuteAndClear();
			EventManager::GetInstance()->GameLoopEvents.PostTick.Execute();
		}
		if(m_TimePassedSinceLastFPSCount >= 1.f)
		{
			m_AverageFPS = m_FrameCount / m_TimePassedSinceLastFPSCount;
			m_TimePassedSinceLastFPSCount = 0.f;
			m_FrameCount = 0.f;
		}else
		{
			m_FrameCount += 1.f;
			m_TimePassedSinceLastFPSCount += DeltaTime;
		}
	}
	
	return !m_Window->ShouldClose();
}

std::shared_ptr<Engine> Application::GetEngine() const
{
	return m_Engine;
}

std::shared_ptr<Window> Application::GetWindow() const
{
	return m_Window;
}

std::shared_ptr<JobSystem> Trickster::Application::GetJobSystem() const
{
	return m_JobSystem;
}

bool Trickster::Application::Paused() const
{
	return m_Paused;
}


