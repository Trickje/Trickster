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
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* a_Window) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		data.OnWindowClose.Execute();
		});
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Width, int a_Height) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		data.OnWindowResize.Execute(a_Width, a_Height);
		});
	glfwSetKeyCallback(m_Window, [](GLFWwindow* a_Window, int a_Key, int a_Keycode, int a_Action, int a_Modifier) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		if(a_Action == GLFW_PRESS)
		{
			data.OnKeyPressed.Execute(a_Key);
		}
		if(a_Action == GLFW_RELEASE)
		{
			data.OnKeyReleased.Execute(a_Key);
		}
		if(a_Action == GLFW_REPEAT)
		{
			data.OnKeyRepeat.Execute(a_Key, 1);
		}
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* a_Window, int a_Button, int a_Action, int a_Mods) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		if(a_Action == GLFW_PRESS)
		{
			data.OnMouseButtonPressed.Execute(a_Button);
		}
		if(a_Action == GLFW_RELEASE)
		{
			data.OnMouseButtonReleased.Execute(a_Button);
		}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		data.OnMouseButtonScrolled.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
		});
	
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		data.OnMouseMoved.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
		});

	glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* a_Window, int a_Iconified) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		if(a_Iconified == GLFW_TRUE)
		{
			data.OnWindowMinimize.Execute(true);

		}else
		{
			data.OnWindowMinimize.Execute(false);
		}
		});
	glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Maximized) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		data.OnWindowMaximize.Execute(a_Maximized);
		});
}


WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{

	glClear(GL_COLOR_BUFFER_BIT);
	
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
	OnWindowClose.AddListener(std::bind(&WindowsWindow::Shutdown, this));
}

void WindowsWindow::Shutdown()
{
	LOG("Shutting down.");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}
