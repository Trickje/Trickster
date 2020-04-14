#include "pch.h"
#include "EventManager.h"

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
		this->Initialize();
	}


	EventManager::~EventManager()
	{
	}

	void EventManager::Initialize()
	{
		LOG("Initializing Events.");
		
	}
}