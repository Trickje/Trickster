#include "Game.h"
#include <EntryPoint.h>



#include "UI/ClickableBox.h"
#include "Events/EventManager.h"
#include "IronMan.h"
#include "Rendering/MeshManager.h"
#include "UI/UIClickable.h"
#include "UI/ProgressBar.h"
using namespace Trickster;
Trickster::Game::Game()
{
	m_IronMan = nullptr;
	box = nullptr;
}

Trickster::Game::~Game()
{
}


void Trickster::Game::OnStart()
{
	m_IronMan = new IronMan("planet_Terrestrial1.obj");
	m_Camera = std::make_shared<Camera>();
	MeshManager::GetInstance()->Initialize(m_Camera);
	m_Camera->SetPosition({ 0.f, 0.f, -10.f });
	m_Camera->LookAt(m_IronMan->GetPosition());
	m_IronMan->Rotate(0.f, 20.f, 0.f);
	box = new UIClickable("Resources/image.png", { 0.f ,0.f }, 600.f, 100.f);
	bar = new ProgressBar();
	bar->SetPercentage(0.f);
	
}

void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	bar->SetPercentage(bar->GetPercentage() + ((1.f / 5.f) * a_DeltaTime));
	float speed = 10.f;
	if(m_Window->GetKeyDown(Keys::W))
	{
		m_Camera->Move(glm::vec3(0.f, 0.f, -1.f) * m_Camera->GetForward() * speed * a_DeltaTime);
	}
	if (m_Window->GetKeyDown(Keys::A))
	{
		m_Camera->Move(glm::vec3(-1.f, 0.f, 0.f) * m_Camera->GetRight()* speed * a_DeltaTime);
	}
	if (m_Window->GetKeyDown(Keys::D))
	{
		m_Camera->Move(glm::vec3(1.f, 0.f, 0.f) * m_Camera->GetRight()* speed * a_DeltaTime);
	}
	if (m_Window->GetKeyDown(Keys::S))
	{
		m_Camera->Move(glm::vec3(0.f, 0.f, 1.f) * m_Camera->GetForward()* speed * a_DeltaTime);
	}
	if (m_Window->GetKeyDown(Keys::SHIFT))
	{
		m_Camera->Move(glm::vec3(0.f, 1.f, 0.f) * m_Camera->GetUp()* speed * a_DeltaTime);
	}
	if (m_Window->GetKeyDown(Keys::SPACE))
	{
		m_Camera->Move(glm::vec3(0.f, -1.f, 0.f) * m_Camera->GetUp()* speed * a_DeltaTime);
	}
	
	m_IronMan->Rotate(45.f * a_DeltaTime);
}


void Game::OnRender()
{
	//this should be empty
}

bool Game::IsTickBased()
{
	return m_TickBased;
}
