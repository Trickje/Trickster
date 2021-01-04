#include "pch.h"
#include "Rendering/TextureManager.h"
#include "Rendering/Texture.h"
namespace Trickster {
	TextureManager* TextureManager::instance = nullptr;



	Texture * TextureManager::GetTexture(const std::string a_TextureName)
	{
		auto it = TextureManager::GetInstance()->map.find(a_TextureName);
		if (it != TextureManager::GetInstance()->map.end())
		{
			//found
			return it->second;
		}
		TextureManager::GetInstance()->map.insert(std::pair<std::string, Trickster::Texture*>(a_TextureName, new Texture(a_TextureName)));

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
		LOG("[TextureManager] Created TextureManager");
	}


	TextureManager::~TextureManager()
	{
	}
}