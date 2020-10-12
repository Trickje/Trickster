#include "pch.h"
#include "WindowsWindow.h"

#include "Events/EventManager.h"

using namespace Trickster;
static bool s_GLFWInitialized = false;
Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}
WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* a_Window) {
		EventManager::GetInstance()->OnWindowClose.Execute();
		});
	
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Width, int a_Height) {
		EventManager::GetInstance()->OnWindowResize.Execute(a_Width, a_Height);
		});
	
	glfwSetKeyCallback(m_Window, [](GLFWwindow* a_Window, int a_Key, int a_Keycode, int a_Action, int a_Modifier) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		if(a_Action == GLFW_PRESS)
		{
			EventManager::GetInstance()->OnKeyPressed.Execute(a_Key);

		}
		if(a_Action == GLFW_RELEASE)
		{
			EventManager::GetInstance()->OnKeyReleased.Execute(a_Key);
		}
		if(a_Action == GLFW_REPEAT)
		{
			EventManager::GetInstance()->OnKeyRepeat.Execute(a_Key, 1);
		}
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* a_Window, int a_Button, int a_Action, int a_Mods) {
		if(a_Action == GLFW_PRESS)
		{
			EventManager::GetInstance()->OnMouseButtonPressed.Execute(a_Button);
		}
		if(a_Action == GLFW_RELEASE)
		{
			EventManager::GetInstance()->OnMouseButtonReleased.Execute(a_Button);
		}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
	
		EventManager::GetInstance()->OnMouseButtonScrolled.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
	});
	
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
		EventManager::GetInstance()->OnMouseMoved.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
		});

	glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* a_Window, int a_Iconified) {
		EventManager::GetInstance()->OnWindowMinimize.Execute(a_Iconified == GLFW_TRUE);
		});
	glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Maximized) {
	
		EventManager::GetInstance()->OnWindowMaximize.Execute(a_Maximized == GLFW_TRUE);
		});
}


WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

}

void WindowsWindow::EventHandle()
{

	
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
	m_vSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
	return m_vSync;
}

void Trickster::WindowsWindow::Draw()
{

	//Render Objects
	
	// Swap buffers
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

bool WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void WindowsWindow::Init(const WindowProps& props)
{
	m_Title = props.title;
	m_Width = props.width;
	m_Height = props.height;

	LOG(std::string("Creating window " + props.title + " " + " (" + std::to_string(props.width) + ", " + std::to_string(props.height) + ")."));
	if(!s_GLFWInitialized)
	{
		LOG("Initializing GLFW.");
		int success = glfwInit();
		ASSERT(success);
		s_GLFWInitialized = true;
	}
	m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);

	LOG("Initializing GLEW.");
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);

	SetVSync(true);
	//Listener to the windowclose.
	EventManager::GetInstance()->OnWindowClose.AddListener(std::bind(&WindowsWindow::Shutdown, this));
}

void WindowsWindow::Shutdown()
{
	LOG("Shutting down.");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}
