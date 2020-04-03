#include "Engine.h"

#include <ctime>


#include "Logger.h"
#include "Renderer.h"
#include "Drawable2D.h"
#include "Nicolai.h"
#include <thread>

Engine::Engine()
{
}


Engine::~Engine()
{
}

//Must be called at entry point
bool Engine::Initialize()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		LOG_ERROR("Failed to initialize GLFW\n");
		return false;
	}
	std::srand(time(0));
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Trickster Engine", nullptr, nullptr);
	if (window == nullptr)
	{
		LOG_ERROR(
			"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	renderer = new Renderer(window);
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
	std::string str = "image.png";
	Drawable2D* drawable = new Drawable2D(glm::vec2(50.f, 120.f),  glm::vec2(40.f, 40.f), str);
	std::vector<Nicolai*> m_Nicolais;
	for(int i = 0; i < 1000; i++)
	{
		m_Nicolais.push_back(new Nicolai(glm::vec2(50.f, 120.f), glm::vec2(40.f, 40.f), str));
	}
	do
	{
		
		//Draw stuff
		//
		if(glfwGetKey(window, GLFW_KEY_SPACE))
		{
			m_Nicolais.push_back( new Nicolai(glm::vec2(50.f, 120.f), glm::vec2(40.f, 40.f), str));
		}
		for(Nicolai* n : m_Nicolais)
		{
			n->OnUpdate();
		}
		renderer->Draw();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
