#include "pch.h"
#include "Nicolai.h"


#include "SpriteManager.h"

#include "Engine.h"
#include "TextureManager.h"
using namespace Trickster;
Nicolai::Nicolai()
{
	
}


Nicolai::~Nicolai()
{
}

Nicolai::Nicolai(glm::vec2 a_Position, glm::vec2 a_Size, std::string& a_FilePath)
{
	m_Position = a_Position;
	m_Size = a_Size;
	m_FilePath = a_FilePath;
	float xDir = static_cast<float>(std::rand() % 100) / 50.f - 0.5f;
	float yDir = static_cast<float>(std::rand() % 100) / 50.f - 0.5f;
	m_Direction = { xDir, yDir };
	SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
}


void Nicolai::OnUpdate()
{
	this->m_Position.x += m_Direction.x;
	this->m_Position.y += m_Direction.y;
	if (this->m_Position.x + 0.5f*m_Size.x > WINDOWWIDTH || this->m_Position.x - (0.5f*m_Size.x) < 0)
	{
		m_Direction = {-m_Direction.x , m_Direction.y};
	}
	if (this->m_Position.y + 0.5f*m_Size.y > WINDOWHEIGHT || this->m_Position.y - (0.5f*m_Size.y) < 0)
	{
		m_Direction = { m_Direction.x , -m_Direction.y };
	}
}

bool Nicolai::OnScreen()
{
	return true;
}
