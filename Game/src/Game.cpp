#include "Game.h"
#include <EntryPoint.h>

#include "IronMan.h"
#include "Nicolai.h"
#include "Rendering/Drawable2D.h"
#include "Rendering/MeshManager.h"
using namespace Trickster;
Trickster::Game::Game()
{
}

Trickster::Game::~Game()
{
}


void Trickster::Game::OnStart()
{
	std::string str = "Models/Terrestrial1.jpg"; //Engine::GetResourcePath() + "image.png";
	//Drawable2D* drawable = new Drawable2D(glm::vec2(50.f, 120.f), glm::vec2(1.f, 1.f), str);
	//m_IronMan = new IronMan("Handgun_obj.obj");
	m_IronMan = new IronMan("planet_Terrestrial1.obj");
	m_Camera = std::make_shared<Camera>();
	MeshManager::GetInstance()->Initialize(m_Camera);
	//m_Camera->LookAt(m_IronMan->GetPosition());
	for (int i = 0; i < 10; i++)
	{
		m_Nicolais.push_back(new Nicolai(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), str));
	}
}

void Trickster::Game::OnUpdate(float a_DeltaTime)
{
	//m_Camera->Move(glm::vec3( 0.f, 0.f, -1.f * a_DeltaTime ));
	m_Camera->Rotate(45.f * a_DeltaTime);
	if (glfwGetKey((GLFWwindow*)this->m_Window.get(), GLFW_KEY_SPACE))
	{
		std::string str = Engine::GetResourcePath() + "image.png";
		m_Nicolais.push_back(new Nicolai(glm::vec2(50.f, 120.f), glm::vec2(40.f, 40.f), str));
	}
	for (Nicolai* n : m_Nicolais)
	{
		n->OnUpdate(a_DeltaTime);
	}
}


void Trickster::Game::OnRender()
{
	//this should be empty
}
