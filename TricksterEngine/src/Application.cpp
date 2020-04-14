#include "pch.h"
#include "Application.h"



#include "../Timer.h"
#include "Engine.h"
#include "Events/EventManager.h"

using namespace Trickster;

Application::Application()
{
	LOG("Started the timer");
	m_Timer.Start();
	m_Engine = std::make_shared<Engine>();
	m_Window = std::unique_ptr<Window>(Window::Create());
	EventManager::GetInstance()->OnStart.AddListener(std::bind(&Application::OnStart, this));
	EventManager::GetInstance()->OnRender.AddListener(std::bind(&Application::OnRender, this));
	EventManager::GetInstance()->OnUpdate.AddListener(std::bind(&Window::OnUpdate, m_Window.get()));
	EventManager::GetInstance()->OnUpdate.AddListener(std::bind(&Application::OnUpdate, this, std::placeholders::_1));

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
EventManager::GetInstance()->OnStart.Execute();
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
	EventManager::GetInstance()->OnUpdate.Execute(DeltaTime);
	return !m_Window->ShouldClose();
}

std::shared_ptr<Engine> Application::GetEngine() const
{
	return m_Engine;
}


