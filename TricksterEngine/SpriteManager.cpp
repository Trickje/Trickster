#include "SpriteManager.h"

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
