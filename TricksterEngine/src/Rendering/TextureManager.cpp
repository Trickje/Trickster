#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
namespace Trickster {
	TextureManager* TextureManager::instance = nullptr;



	Texture * TextureManager::GetTexture(const std::string texturePath)
	{
		auto it = TextureManager::GetInstance()->map.find(texturePath);
		if (it != TextureManager::GetInstance()->map.end())
		{
			//found
			return it->second;
		}
		TextureManager::GetInstance()->map.insert(std::pair<std::string, Trickster::Texture*>(texturePath, new Texture(texturePath)));

		return TextureManager::GetInstance()->map[texturePath];
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
		LOG("[TextureManager] Created TextureManager");
	}


	TextureManager::~TextureManager()
	{
	}
}