#include "pch.h"
#include "Game.h"

#include "Nicolai.h"
#include "Rendering/Drawable2D.h"
using namespace Trickster;
Trickster::Game::Game()
{
}

Trickster::Game::~Game()
{
}



void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	//Draw stuff
			//
	if (glfwGetKey((GLFWwindow*)this->m_Window.get(), GLFW_KEY_SPACE))
	{
		std::string str = Engine::GetResourcePath() + "image.png";
		m_Nicolais.push_back(new Nicolai(glm::vec2(50.f, 120.f), glm::vec2(40.f, 40.f), str));
	}
	for (Nicolai* n : m_Nicolais)
	{
		n->OnUpdate();
	}
}

void Trickster::Game::OnStart()
{
	std::string str = Engine::GetResourcePath() + "image.png";
	Drawable2D* drawable = new Drawable2D(glm::vec2(50.f, 120.f), glm::vec2(1.f, 1.f), str);

	for (int i = 0; i < 10; i++)
	{
		m_Nicolais.push_back(new Nicolai(glm::vec2(50.f, 120.f), glm::vec2(1.f, 1.f), str));
	}
}

void Trickster::Game::OnRender()
{

	GetEngine()->renderer->Draw();
}
