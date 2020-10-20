#pragma once
#include <string>

#include "Rendering/Drawable3D.h"
#include <glm/vec3.hpp>

class IronMan :
	public Trickster::Drawable3D
{
public:
	IronMan(const std::string& a_ModelFileName = "IronMan.obj", const glm::vec3& a_Position = {0.f, 0.f, 0.f}, const glm::vec3& a_Scale = { 1.f,1.f,1.f }, const std::string& a_ShaderFileName = "");
	~IronMan();
};

