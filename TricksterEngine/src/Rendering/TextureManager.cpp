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
		return TextureManager::GetInstance()->map[texturePath] = new Texture(texturePath);

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
	}


	TextureManager::~TextureManager()
	{
	}
}