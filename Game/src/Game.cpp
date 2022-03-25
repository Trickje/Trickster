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
#include "Game.h"

#include "Rendering/Window.h"
#include "Rendering/MeshManager.h"
#include "Events/EventManager.h"
#include "Core/Input.h"
#include "IronMan.h"
#include "Core/EntryPoint.h"
#include "Core/Version.h"
//#include <Network/Package.h>
//#include <Network/Network.h>

#include "Rendering/RenderAPI/RenderAPI.h"
#include "Rendering/Text/TextRenderer.h"
using namespace TE;
Game::Game()
{
	m_Camera = std::make_shared<Camera>();
	MeshManager::GetInstance()->Initialize(m_Camera);
//	m_IronMan = nullptr;
//	bar = nullptr;
}

Game::~Game()
{
}


void Game::OnStart()
{
	m_Window->SetTitle({ "Trickster Engine v" + TRICKSTER_VERSION_STRING });
	m_MainMusic = m_AudioPlayer->Play("Music/MainMenu/minecraft.mp3");
	m_AudioPlayer->SetVolume(m_MainMusic, 0.5f);
	m_Camera->SetPosition({ 0.f, 0.f, 10.f });
	m_Camera->LookAt({ 0.f,0.f,0.f });
	
	 
	m_Objects.push_back(new Drawable3D{ "planet_Terrestrial1.obj", {0.5f, 0.f, -5.f} });
	m_Objects.push_back(new Drawable3D{ "planet_Terrestrial1.obj", {0.f,0.f, -5.f} });
	m_Objects.push_back(new Drawable3D{ "planet_Terrestrial1.obj", {0.f,0.5f, -5.f} });
	m_Objects.push_back(new Drawable3D{ "viking_room.obj", {0.f,-0.5f, -5.f} });
	m_Sprites.push_back(new Drawable2D("PauseMenu.png", {0.f,0.f}, {0.f,0.f}));
	m_Sprites[0]->SetVisible(false);
	EventManager::GetInstance()->InputEvents.OnMouseMoved.AddListener(std::bind(&Camera::MouseMove, m_Camera.get(), std::placeholders::_1,std::placeholders::_2));

}

void Game::OnUpdate(float a_DeltaTime)
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
		
	}
	
	
	
	TextRenderer::Get()->RenderString("FPS: " + std::to_string((int)m_AverageFPS), { 0,GetWindow()->GetHeight() - 48.f },1.f, {0.2f, 0.2f, 0.2f, 0.5f});
	TextRenderer::Get()->RenderString("1234567890", {100.f, 200.f});
}


void Game::OnRender()
{
	//this should be empty
}

void Game::OnPause(bool isPaused)
{
	m_Sprites[0]->SetVisible(isPaused);
}

bool Game::IsTickBased()
{
	return m_TickBased;
}


