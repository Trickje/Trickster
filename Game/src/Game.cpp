#include "Game.h"
#include <EntryPoint.h>



#include "UI/ClickableBox.h"
#include "Events/EventManager.h"
#include "Input.h"
#include "IronMan.h"
#include "Rendering/MeshManager.h"
#include "UI/UIClickable.h"
#include "UI/ProgressBar.h"
using namespace Trickster;
Trickster::Game::Game()
{
	m_IronMan = nullptr;
	box = nullptr;
	bar = nullptr;
	box2 = nullptr;
}

Trickster::Game::~Game()
{
}


void Trickster::Game::OnStart()
{
	m_IronMan = new IronMan("planet_Terrestrial1.obj");
	m_Camera = std::make_shared<Camera>();
	MeshManager::GetInstance()->Initialize(m_Camera);
	m_Camera->SetPosition({ 0.f, 0.f, 10.f });
	m_Camera->LookAt(m_IronMan->GetPosition());
	m_IronMan->Rotate(0.f, 20.f, 0.f);
	box = new UIClickable("Resources/image.png", { 0.f ,0.f }, 600.f, 100.f);
	box->SetPosition({ (Application::Get()->GetWindow()->GetWidth() / 2.f) - 100.f, Application::Get()->GetWindow()->GetHeight() / 2.f });
	box2 = new UIClickable("Resources/image.png", {( Application::Get()->GetWindow()->GetWidth() / 2.f) - 100.f, Application::Get()->GetWindow()->GetHeight() / 2.f  - box->GetHeight()},  100, box->GetHeight());
	bar = new ProgressBar();
	EventManager::GetInstance()->OnMouseMoved.AddListener(std::bind(&Camera::MouseMove, m_Camera.get(), std::placeholders::_1,std::placeholders::_2));
	bar->SetPosition({ 50.f, 30.f });
	bar->SetSize({20.f, 5.f});
	//bar->SetColorFront({1.f, 0.f, 0.f});
	//bar->SetColorBack({0.f, 1.f, 0.f});
	bar->SetPercentage(1.f);
	//bar->SetPosition({ Application::GetWindow()->GetWidth() - (bar->GetSize().x * bar->GetScale().x ), Application::GetWindow()->GetHeight() - (bar->GetSize().y * bar->GetScale().y)});
}

void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	bar->SetPosition({ bar->GetPosition().x, bar->GetPosition().y + 5.f * a_DeltaTime});
	if(box->isClicked())
	{
		bar->SetPercentage(bar->GetPercentage() -0.2f);
	}
	float speed = 10.f;
	if(Input::GetKeyDown(Keys::W))
	{
		m_Camera->Move(1.f * m_Camera->GetForward() * speed * a_DeltaTime);
	}
	if (Input::GetKeyDown(Keys::A))
	{
		m_Camera->Move(-1.f * m_Camera->GetRight()* speed * a_DeltaTime);
	}
	if (Input::GetKeyDown(Keys::D))
	{
		m_Camera->Move(1.f  * m_Camera->GetRight()* speed * a_DeltaTime);
	}
	if (Input::GetKeyDown(Keys::S))
	{
		m_Camera->Move(-1.f * m_Camera->GetForward()* speed * a_DeltaTime);
	}
	if (Input::GetKeyDown(Keys::SHIFT))
	{
		m_Camera->Move(glm::vec3(0.f, -1.f, 0.f) * speed * a_DeltaTime);
	}
	if (Input::GetKeyDown(Keys::SPACE))
	{
		m_Camera->Move(glm::vec3(0.f, 1.f, 0.f) * speed * a_DeltaTime);
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
