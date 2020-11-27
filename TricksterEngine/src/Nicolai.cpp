#include "pch.h"
#include "Nicolai.h"
#include "SpriteManager.h"
using namespace Trickster;
Nicolai::Nicolai()
{
	m_Direction = {};
}


Nicolai::~Nicolai()
{
}

Nicolai::Nicolai(glm::vec2 a_Position, glm::vec2 a_Scale, std::string& a_FilePath)
{
	m_Position = a_Position;
	m_Scale = a_Scale;
	m_FilePath = a_FilePath;
	float xDir = static_cast<float>(std::rand() % 100) / 50.f - 0.5f;
	float yDir = static_cast<float>(std::rand() % 100) / 50.f - 0.5f;
	m_Direction = { xDir, yDir };
	SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
}


void Nicolai::OnUpdate(float a_DeltaTime)
{
	
}

bool Nicolai::OnScreen()
{
	return true;
}
