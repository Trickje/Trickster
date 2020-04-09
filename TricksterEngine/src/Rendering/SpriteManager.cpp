#include "pch.h"
#include "SpriteManager.h"
namespace Trickster {
	SpriteManager* SpriteManager::instance = nullptr;
	SpriteManager * SpriteManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new SpriteManager();
		}
		return instance;
	}

	SpriteManager::SpriteManager()
	{
	}


	SpriteManager::~SpriteManager()
	{
	}
}