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
		/*
		LOG("Initializing GLFW.");
		// Initialise GLFW
		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW\n");
			return false;
		}
		std::srand(static_cast<unsigned int>(time(0)));
		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

		LOG("Opening window.");
		// Open a window and create its OpenGL context
		window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Trickster Engine", nullptr, nullptr);
		if (window == nullptr)
		{
			LOG_ERROR(
				"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			glfwTerminate();
			return false;
		}
		LOG("Initializing GLEW.");
		glfwMakeContextCurrent(window); // Initialize GLEW
		glewExperimental = GL_TRUE;
		glewInit();
		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		*/
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