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
#include "Rendering/ShaderManager.h"
#include "Rendering/Shader.h"


namespace TE {
	ShaderManager* ShaderManager::instance = nullptr;



	Shader * ShaderManager::GetShader(const std::string a_FilePath)
	{
		auto it = ShaderManager::GetInstance()->map.find(a_FilePath);
		if (it != ShaderManager::GetInstance()->map.end())
		{
			//found
			return it->second;
		}
		return ShaderManager::GetInstance()->map[a_FilePath] = new Shader(a_FilePath);

	}





	ShaderManager * ShaderManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ShaderManager();
		}
		return instance;
	}

	void ShaderManager::Initialize()
	{
		GetShader("basic")->Bind();
		GetShader("basic")->SetUniform1i("ourTexture", 0);
		GetShader("3D");
	}

	ShaderManager::ShaderManager()
	{
#ifdef DETAILED_CONSOLE
		LOG("[ShaderManager] Created ShaderManager");
#endif
	}


	ShaderManager::~ShaderManager()
	{
	}
}