#include "pch.h"
#include "Events/EventManager.h"
namespace Trickster {
	EventManager* EventManager::instance = nullptr;
	EventManager * EventManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new EventManager();
		}
		return instance;
	}
	EventManager::EventManager()
	{
	}


	EventManager::~EventManager()
	{
	}

}
