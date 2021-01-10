#include "pch.h"
#include "Game.h"


#include "Rendering/Window.h"
#include "UI/ClickableBox.h"
#include "Events/EventManager.h"
#include "Core/Input.h"
#include "IronMan.h"
#include "Rendering/MeshManager.h"
#include "UI/UIClickable.h"
#include "UI/ProgressBar.h"
#include "Core/EntryPoint.h"
#include "Core/Version.h"
#include <Network/Package.h>
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
	m_Window->SetTitle({ "Trickster Engine v" + TRICKSTER_VERSION_STRING });
	m_IronMan = new IronMan("planet_Terrestrial1.obj");
	m_PauseMenu = new UIClickable("PauseMenu.png", {0.f,0.f}, static_cast<float>(m_Window->GetWidth()), static_cast<float>(m_Window->GetHeight()));
	m_PauseMenu->SetVisible(false);
	m_Camera = std::make_shared<Camera>();
	m_MainMusic = m_AudioPlayer->Play("Music/MainMenu/minecraft.mp3");
	m_AudioPlayer->SetVolume(m_MainMusic, 0.5f);
	MeshManager::GetInstance()->Initialize(m_Camera);
	m_Camera->SetPosition({ 0.f, 0.f, 10.f });
	m_Camera->LookAt(m_IronMan->GetPosition());
	Message msg;
	msg += std::string("Hello ");
	msg += "world";
	msg += '!';
	LOG(msg.to_string());
	for(int i = 0; i < 10; i++)
	{
		m_Planets.push_back(new IronMan("planet_Terrestrial1.obj"));
		m_Planets[m_Planets.size() - 1]->Rotate(0.f, 20.f, 0.f);
		m_Planets[m_Planets.size() - 1]->Move({5 * i, 0, 0});
	}
	m_IronMan->Rotate(0.f, 20.f, 0.f);
	EventManager::GetInstance()->InputEvents.OnMouseMoved.AddListener(std::bind(&Camera::MouseMove, m_Camera.get(), std::placeholders::_1,std::placeholders::_2));

}

void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	float volume = -1.f;
	volume = m_AudioPlayer->GetVolume(m_MainMusic);
	if (volume < 1.f) {
		m_AudioPlayer->ChangeVolume(m_MainMusic, 0.2f * a_DeltaTime);
	}else
	{
		m_AudioPlayer->Pause(m_MainMusic);
		//LOG("Music paused");
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


