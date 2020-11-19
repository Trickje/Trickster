#include "pch.h"
#include "WindowsWindow.h"

#include "Events/EventManager.h"
#include <functional>

#include "stb_image.h"
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
	//	EventManager::GetInstance()->OnWindowResize.Execute(a_Width, a_Height);
		});
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Width, int a_Height) {
		EventManager::GetInstance()->OnWindowResize.Execute(a_Width, a_Height);
		});
	glfwSetWindowRefreshCallback(m_Window, [](GLFWwindow* a_Window)
	{
			WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
			data.Draw();
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
	if(GetKeyDown(Keys::CTRL))
	{
		SetFullscreen(true);
	}
	if(GetKeyDown(Keys::TAB))
	{
		SetFullscreen(false);
	}
}

void WindowsWindow::EventHandle()
{

	glfwPollEvents();
	
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

	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//Render Objects
	EventManager::GetInstance()->OnRender.Execute();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	EventManager::GetInstance()->OnRenderTransparent.Execute();
	glDisable(GL_BLEND);
	// Swap buffers
	glfwSwapBuffers(m_Window);
}

bool WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

bool Trickster::WindowsWindow::GetKeyDown(int Key)
{
	if(Key >= 54)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(Key + 1) + " out of 53.");
		return false;
	}
	return Keys[Key];
}

void Trickster::WindowsWindow::SetKeyDown(int Key, bool a_bool)
{
	switch(Key)
	{
	case GLFW_KEY_Q:
		Keys[Keys::Q] = a_bool;
		break;
	case GLFW_KEY_W:
		Keys[Keys::W] = a_bool;
		break;
	case GLFW_KEY_E:
		Keys[Keys::E] = a_bool;
		break;
	case GLFW_KEY_R:
		Keys[Keys::R] = a_bool;
		break;
	case GLFW_KEY_T:
		Keys[Keys::T] = a_bool;
		break;
	case GLFW_KEY_Y:
		Keys[Keys::Y] = a_bool;
		break;
	case GLFW_KEY_U:
		Keys[Keys::U] = a_bool;
		break;
	case GLFW_KEY_I:
		Keys[Keys::I] = a_bool;
		break;
	case GLFW_KEY_O:
		Keys[Keys::O] = a_bool;
		break;
	case GLFW_KEY_P:
		Keys[Keys::P] = a_bool;
		break;
	case GLFW_KEY_A:
		Keys[Keys::A] = a_bool;
		break;
	case GLFW_KEY_S:
		Keys[Keys::S] = a_bool;
		break;
	case GLFW_KEY_D:
		Keys[Keys::D] = a_bool;
		break;
	case GLFW_KEY_F:
		Keys[Keys::F] = a_bool;
		break;
	case GLFW_KEY_G:
		Keys[Keys::G] = a_bool;
		break;
	case GLFW_KEY_H:
		Keys[Keys::H] = a_bool;
		break;
	case GLFW_KEY_J:
		Keys[Keys::J] = a_bool;
		break;
	case GLFW_KEY_K:
		Keys[Keys::K] = a_bool;
		break;
	case GLFW_KEY_L:
		Keys[Keys::L] = a_bool;
		break;
	case GLFW_KEY_Z:
		Keys[Keys::Z] = a_bool;
		break;
	case GLFW_KEY_X:
		Keys[Keys::X] = a_bool;
		break;
	case GLFW_KEY_C:
		Keys[Keys::C] = a_bool;
		break;
	case GLFW_KEY_V:
		Keys[Keys::V] = a_bool;
		break;
	case GLFW_KEY_B:
		Keys[Keys::B] = a_bool;
		break;
	case GLFW_KEY_N:
		Keys[Keys::N] = a_bool;
		break;
	case GLFW_KEY_M:
		Keys[Keys::M] = a_bool;
		break;
	case GLFW_KEY_TAB:
		Keys[Keys::TAB] = a_bool;
		break;
	case GLFW_KEY_CAPS_LOCK:
		Keys[Keys::CAPSLOCK] = a_bool;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		Keys[Keys::SHIFT] = a_bool;
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		Keys[Keys::SHIFT] = a_bool;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		Keys[Keys::CTRL] = a_bool;
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		Keys[Keys::CTRL] = a_bool;
		break;
	case GLFW_KEY_LEFT_ALT:
		Keys[Keys::ALT] = a_bool;
		break;
	case GLFW_KEY_RIGHT_ALT:
		Keys[Keys::ALT] = a_bool;
		break;
	case GLFW_KEY_SPACE:
		Keys[Keys::SPACE] = a_bool;
		break;
	case GLFW_KEY_1:
		Keys[Keys::ONE] = a_bool;
		break;
	case GLFW_KEY_2:
		Keys[Keys::TWO] = a_bool;
		break;
	case GLFW_KEY_3:
		Keys[Keys::THREE] = a_bool;
		break;
	case GLFW_KEY_4:
		Keys[Keys::FOUR] = a_bool;
		break;
	case GLFW_KEY_5:
		Keys[Keys::FIVE] = a_bool;
		break;
	case GLFW_KEY_6:
		Keys[Keys::SIX] = a_bool;
		break;
	case GLFW_KEY_7:
		Keys[Keys::SEVEN] = a_bool;
		break;
	case GLFW_KEY_8:
		Keys[Keys::EIGHT] = a_bool;
		break;
	case GLFW_KEY_9:
		Keys[Keys::NINE] = a_bool;
		break;
	case GLFW_KEY_0:
		Keys[Keys::ZERO] = a_bool;
		break;
	case GLFW_KEY_MINUS:
		Keys[Keys::MINUS] = a_bool;
		break;
	case GLFW_KEY_EQUAL:
		Keys[Keys::EQUALS] = a_bool;
		break;
	case GLFW_KEY_BACKSPACE:
		Keys[Keys::BACKSPACE] = a_bool;
		break;
	case GLFW_KEY_COMMA:
		Keys[Keys::LESS_THAN] = a_bool;
		break;
	case GLFW_KEY_PERIOD:
		Keys[Keys::GREATER_THAN] = a_bool;
		break;
	case GLFW_KEY_SLASH:
		Keys[Keys::SLASH] = a_bool;
		break;
	case GLFW_KEY_ESCAPE:
		Keys[Keys::ESC] = a_bool;
		break;
	case GLFW_KEY_ENTER:
		Keys[Keys::ENTER] = a_bool;
		break;
	
	}
}

void Trickster::WindowsWindow::SetFullscreen(bool a_Fullscreen)
{
	if (a_Fullscreen) {
		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, GetWidth(), GetHeight(), GLFW_DONT_CARE);
	}else
	{
		glfwSetWindowMonitor(m_Window, nullptr, m_PosX, m_PosY, GetWidth(), GetHeight(), GLFW_DONT_CARE);
		glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
	}
}

void Trickster::WindowsWindow::GetCursorPos(double* x, double* y)
{
	glfwGetCursorPos(m_Window,x, y);
}

bool Trickster::WindowsWindow::GetClick(int MouseKey)
{
	if(MouseKey >= 8)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(MouseKey + 1) + " out of 53.");
	}
	return MouseKeys[MouseKey];
}

void Trickster::WindowsWindow::SetClick(int MouseKey, bool value)
{
	switch(MouseKey)
	{
	case GLFW_MOUSE_BUTTON_1:
		MouseKeys[Mouse::Left] = value;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		MouseKeys[Mouse::Middle] = value;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		MouseKeys[Mouse::Right] = value;
		break;
	case GLFW_MOUSE_BUTTON_4:
		MouseKeys[Mouse::Four] = value;
		break;
	case GLFW_MOUSE_BUTTON_5:
		MouseKeys[Mouse::Five] = value;
		break;
		
	}
	if(MouseKey >= 8)
	{
		LOG_ERROR("Not supposed to happen");
	}
	MouseKeys[MouseKey] = value;
}

void Trickster::WindowsWindow::Resize(float a_Width, float a_Height)
{
	m_Width = a_Width;
	m_Height = a_Height;
	//int left, top, right, bottom;
	//glfwGetWindowFrameSize(m_Window, &left, &top, &right, &bottom);
	glViewport(0, 0, m_Width, m_Height);
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
	//Loading in the icon
	int i_Width, i_Height, i_BPP;
	GLFWimage image[1];
	unsigned char* i_LocalBuffer = stbi_load("Resources/Icon/icon.png", &i_Width, &i_Height, &i_BPP, 0);
	image[0].height = i_Height;
	image[0].width = i_Width;
	image[0].pixels = i_LocalBuffer;
	glfwSetWindowIcon(m_Window, 1, image);
	glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);

	LOG("Initializing GLEW.");
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	SetVSync(true);
	//Listener to the windowclose.
	EventManager::GetInstance()->OnKeyPressed.AddListener(std::bind(&WindowsWindow::SetKeyDown, this, std::placeholders::_1, true));
	EventManager::GetInstance()->OnKeyReleased.AddListener(std::bind(&WindowsWindow::SetKeyDown, this, std::placeholders::_1, false));
	EventManager::GetInstance()->OnWindowClose.AddListener(std::bind(&WindowsWindow::Shutdown, this));
	EventManager::GetInstance()->OnMouseButtonPressed.AddListener(std::bind(&WindowsWindow::SetClick, this, std::placeholders::_1, true));
	EventManager::GetInstance()->OnMouseButtonReleased.AddListener(std::bind(&WindowsWindow::SetClick, this, std::placeholders::_1, false));
	EventManager::GetInstance()->OnWindowResize.AddListener(std::bind(&WindowsWindow::Resize, this, std::placeholders::_1, std::placeholders::_2));
}

void WindowsWindow::Shutdown()
{
	LOG("Shutting down.");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}
