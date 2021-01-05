#include "pch.h"
#include "Rendering/SpriteManager.h"
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
#ifdef DETAILED_CONSOLE
		LOG("[SpriteManager] Created SpriteManager");
#endif
	}


	SpriteManager::~SpriteManager()
	{
	}
}