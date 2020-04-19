#pragma once
#include "Rendering/Drawable2D.h"
using namespace Trickster;
class Nicolai :
	public Drawable2D
{
public:
	Nicolai();
	~Nicolai();
	Nicolai(glm::vec2 a_Position, glm::vec2 a_Scale, std::string& a_FilePath);
	void OnUpdate();
	bool OnScreen();
private:
	glm::vec2 m_Direction;
};

