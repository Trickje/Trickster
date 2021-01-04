#include "pch.h"
#include "Core/Engine.h"

#include "Rendering/Renderer.h"
namespace Trickster
{

	Engine::Engine()
	{
		renderer = nullptr;
	}


	Engine::~Engine()
	{
		delete renderer;
	}

	//Must be called at entry point
	bool Engine::Initialize()
	{
		renderer = new Renderer();
		if (!renderer->Initialize())
		{
			LOG_ERROR("Failed to initialize the renderer");
		}
#ifdef __DEBUG
		glDebugMessageCallback(DebugMessageCallback, 0);
#endif
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