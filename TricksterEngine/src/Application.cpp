#include "pch.h"
#include "Application.h"



#include "../Timer.h"
#include "Engine.h"

using namespace Trickster;

Application::Application()
{
	m_Engine = std::make_shared<Engine>();
	m_Window = std::unique_ptr<Window>(Window::Create());
	LOG("Started the timer");
	m_Timer.Start();
}


Application::~Application()
{
//m_Engine implicitly deleted
}

void Application::Start()
{
	LOG("Initializing program.");
if (!m_Engine->Initialize())
{
	LOG_ERROR("Failed to initialize the engine!");
}

std::srand(static_cast<unsigned int>(time(0)));
OnStart();
	float time_passed = m_Timer.GetSeconds();
	if (time_passed < 1.f) {
		LOG("Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
	}else
	{
		LOG_WARNING("Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
	}
	m_Timer.Reset();
	
}

void Application::Draw()
{
	m_Window->Draw();
}

bool Trickster::Application::Update()
{
	float DeltaTime = m_Timer.Reset();
	//DELTA TIME calculations
	m_Window->OnUpdate();
	OnUpdate(DeltaTime);
	return !m_Window->ShouldClose();
}

std::shared_ptr<Engine> Application::GetEngine() const
{
	return m_Engine;
}


