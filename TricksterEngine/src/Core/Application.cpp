#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "ShaderManager.h"
using namespace Trickster;

Application* Application::m_Application = nullptr;
Application::Application()
{
	LOG("[Application] Starting initialization");
	m_Timer.Start();
	m_Engine = std::make_shared<Engine>();
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Jobsystem = std::make_unique<Jobsystem>();
	EventManager::GetInstance()->GameLoopEvents.OnStart.AddListener(std::bind(&Application::OnStart, this));
	EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Application::OnRender, this));
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&Window::OnUpdate, m_Window.get()));
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&Application::OnUpdate, this, std::placeholders::_1));
	EventManager::GetInstance()->GameLoopEvents.OnStart.AddListener(std::bind(&ShaderManager::Initialize, ShaderManager::GetInstance()));
	m_Application = this;
}


Application::~Application()
{
//m_Engine implicitly deleted
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
std::srand(static_cast<unsigned int>(time(0)));
EventManager::GetInstance()->GameLoopEvents.OnStart.Execute();
	float time_passed = m_Timer.GetSeconds();
	if (time_passed < 1.f) {
		LOG("[Application] Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
	}else
	{
		LOG_WARNING("[Application] Initialization complete!\nTook " + std::to_string(time_passed) + " seconds.");
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
	float DeltaTime = m_Timer.Reset();
	//DELTA TIME calculations
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.Execute(DeltaTime);
	CurrentTickTime += DeltaTime;
	while(CurrentTickTime >= TickTime)
	{
		CurrentTickTime -= TickTime;
		EventManager::GetInstance()->GameLoopEvents.PreTick.Execute();
		EventManager::GetInstance()->GameLoopEvents.Tick.Execute();
		EventManager::GetInstance()->GameLoopEvents.TickOnce.ExecuteAndClear();
		EventManager::GetInstance()->GameLoopEvents.PostTick.Execute();
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


