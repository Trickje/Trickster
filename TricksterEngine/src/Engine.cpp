#include "pch.h"
#include "Engine.h"

#include <ctime>


#include "Renderer.h"
#include "Drawable2D.h"
#include "Nicolai.h"
#include <thread>

namespace Trickster
{

	Engine::Engine()
	{
	}


	Engine::~Engine()
	{
	}

	//Must be called at entry point
	bool Engine::Initialize()
	{
		LOG("Initializing Renderer.");
		renderer = new Renderer();
		if (!renderer->Initialize())
		{
			LOG_ERROR("Failed to initialize the renderer");
		}

		glDebugMessageCallback(DebugMessageCallback, 0);

		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		return true;
	}

	void Engine::Update()
	{
		
		//Something?
	}
	std::string Engine::GetResourcePath()
	{
		return "../../TricksterEngine/TricksterEngine/Resources/";
	}

}