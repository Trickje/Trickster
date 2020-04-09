#include "Application.h"



#include "../Timer.h"
#include "Engine.h"
#include "Logger.h"

using namespace Trickster;

Application::Application()
{
	m_Engine = std::make_shared<Engine>();
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

OnStart();
}

bool Trickster::Application::Update()
{
	float DeltaTime = m_Timer.Reset();
	//DELTA TIME calculations
	OnUpdate(DeltaTime);
	return !glfwWindowShouldClose(m_Engine->window);
}

std::shared_ptr<Engine> Application::GetEngine() const
{
	return m_Engine;
}


