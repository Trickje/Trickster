#include "pch.h"
#include "Engine.h"

#include "Rendering/Renderer.h"
namespace Trickster
{

	Engine::Engine()
	{
		renderer = nullptr;
	}


	Engine::~Engine()
	{
	}

	//Must be called at entry point
	bool Engine::Initialize()
	{
		Profiler profiler("Engine Initialize");
		LOG("Initializing Renderer.");
		renderer = new Renderer();
		if (!renderer->Initialize())
		{
			LOG_ERROR("Failed to initialize the renderer");
		}

		glDebugMessageCallback(DebugMessageCallback, 0);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		return true;
	}

	void Engine::Update()
	{
		
		//Something?
	}
	std::string Engine::GetResourcePath()
	{
		return "Resources/";
	}

}