#include "pch.h"
#include "WindowsWindow.h"

using namespace Trickster;
static bool s_GLFWInitialized = false;
Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}
WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
}


WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{

	glClear(GL_COLOR_BUFFER_BIT);
}

void WindowsWindow::SetVSync(bool enabled)
{
	if(enabled)
	{
		glfwSwapInterval(1);
	}else
	{
		glfwSwapInterval(0);
	}
	m_Data.vSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
	return m_Data.vSync;
}

void Trickster::WindowsWindow::Draw()
{

	//Render Objects
	
	// Swap buffers
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void WindowsWindow::Init(const WindowProps& props)
{
	m_Data.title = props.title;
	m_Data.width = props.width;
	m_Data.height = props.height;

	LOG(std::string("Creating window " + props.title + " " + " (" + std::to_string(props.width) + ", " + std::to_string(props.height) + ")."));
	if(!s_GLFWInitialized)
	{
		LOG("Initializing GLFW.");
		int success = glfwInit();
		ASSERT(success);
		s_GLFWInitialized = true;
	}
	m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);

	LOG("Initializing GLEW.");
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);

	SetVSync(true);
}

void WindowsWindow::Shutdown()
{
	
	glfwDestroyWindow(m_Window);
}
