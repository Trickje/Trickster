#pragma once
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

