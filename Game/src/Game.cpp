#include "pch.h"
#include "Game.h"
#include <Core/EntryPoint.h>



#include "UI/ClickableBox.h"
#include "Events/EventManager.h"
#include "Core/Input.h"
#include "IronMan.h"
#include "Rendering/MeshManager.h"
#include "UI/UIClickable.h"
#include "UI/ProgressBar.h"
using namespace Trickster;
Trickster::Game::Game()
{
	m_IronMan = nullptr;
	bar = nullptr;
}

Trickster::Game::~Game()
{
}


void Trickster::Game::OnStart()
{
	m_IronMan = new IronMan("planet_Terrestrial1.obj");
	m_PauseMenu = new UIClickable("PauseMenu.png", {0.f,0.f}, m_Window->GetWidth(), m_Window->GetHeight());
	m_PauseMenu->SetVisible(false);
	m_Camera = std::make_shared<Camera>();
	m_AudioPlayer->LoadSound("Music/MainMenu/FortunateSon.mp3");
	m_MainMusic = m_AudioPlayer->Play("Music/MainMenu/FortunateSon.mp3");
	m_AudioPlayer->SetVolume(m_MainMusic, 0.f);
	MeshManager::GetInstance()->Initialize(m_Camera);
	m_Camera->SetPosition({ 0.f, 0.f, 10.f });
	m_Camera->LookAt(m_IronMan->GetPosition());

	for(int i = 0; i < 10; i++)
	{
		m_Planets.push_back(new IronMan("planet_Terrestrial1.obj"));
		m_Planets[m_Planets.size() - 1]->Rotate(0.f, 20.f, 0.f);
		m_Planets[m_Planets.size() - 1]->Move({5 * i, 0, 0});
	}
	m_IronMan->Rotate(0.f, 20.f, 0.f);
	//bar = new ProgressBar();
	EventManager::GetInstance()->InputEvents.OnMouseMoved.AddListener(std::bind(&Camera::MouseMove, m_Camera.get(), std::placeholders::_1,std::placeholders::_2));
//	bar->SetPosition({ 50.f, 30.f });
//	bar->SetSize({20.f, 5.f});
	//bar->SetColorFront({1.f, 0.f, 0.f});
	//bar->SetColorBack({0.f, 1.f, 0.f});
//	bar->SetPercentage(1.f);
	//bar->SetPosition({ Application::GetWindow()->GetWidth() - (bar->GetSize().x * bar->GetScale().x ), Application::GetWindow()->GetHeight() - (bar->GetSize().y * bar->GetScale().y)});
}

void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	float volume = -1.f;
	m_AudioPlayer->GetVolume(m_MainMusic, &volume);
	if (volume < 0.5f) {
		m_AudioPlayer->ChangeVolume(m_MainMusic, 0.01f * a_DeltaTime);
		LOG_USELESS(volume);
	}float speed = 10.f;
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
		for(auto& p : m_Planets)
		{
			LOG("x " + std::to_string(p->GetPosition().x) + "   y: " + std::to_string(p->GetPosition().y) + "    z: " + std::to_string(p->GetPosition().z));
		}
	}
	
	
	
	m_IronMan->Rotate(45.f * a_DeltaTime);
	for(auto& p : m_Planets)
	{
		p->Rotate(45.f * a_DeltaTime);
	}
}


void Game::OnRender()
{
	//this should be empty
}

void Trickster::Game::OnPause(bool isPaused)
{
	m_PauseMenu->SetVisible(isPaused);
}

bool Game::IsTickBased()
{
	return m_TickBased;
}


