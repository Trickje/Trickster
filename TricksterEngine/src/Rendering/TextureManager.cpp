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
#include "Rendering/TextureManager.h"
#include "Rendering/Texture.h"
namespace TE {
	TextureManager* TextureManager::instance = nullptr;



	Texture * TextureManager::GetTexture(const std::string a_TextureName)
	{
		auto it = TextureManager::GetInstance()->map.find(a_TextureName);
		if (it != TextureManager::GetInstance()->map.end())
		{
			//found
			return it->second;
		}
		TextureManager::GetInstance()->map.insert(std::pair<std::string, Texture*>(a_TextureName, new Texture(a_TextureName)));

		return TextureManager::GetInstance()->map[a_TextureName];
	}





	TextureManager * TextureManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new TextureManager();
		}
		return instance;
	}

	TextureManager::TextureManager()
	{
#ifdef DETAILED_CONSOLE
		LOG("[TextureManager] Created TextureManager");
#endif
	}


	TextureManager::~TextureManager()
	{
	}
}