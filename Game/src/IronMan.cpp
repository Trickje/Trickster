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
#include "IronMan.h"

#include <string>
#include <glm/vec3.hpp>

IronMan::IronMan(const std::string & a_ModelFileName, const glm::vec3 & a_Position, const glm::vec3 & a_Scale, const std::string & a_ShaderFileName)
{
	if(a_ShaderFileName.empty())
	{
		Initialize(a_ModelFileName, a_Position, a_Scale);
	}else
	{
		Initialize(a_ModelFileName, a_Position, a_Scale, a_ShaderFileName);
	}
}


IronMan::~IronMan()
{
}
