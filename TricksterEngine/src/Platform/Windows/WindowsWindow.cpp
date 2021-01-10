/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
#include "pch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Events/EventManager.h"
#include "stb_image.h"

using namespace Trickster;
static bool s_GLFWInitialized = false;
#ifdef TRICKSTER_OPENGL
Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}
#endif
WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* a_Window) {
		EventManager::GetInstance()->WindowEvents.OnWindowClose.Execute();
		});
	
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Width, int a_Height) {
		EventManager::GetInstance()->WindowEvents.OnWindowResize.Execute(a_Width, a_Height);
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		Application::Get()->Update();
		data.Draw();
		});
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Width, int a_Height) {
		EventManager::GetInstance()->WindowEvents.OnWindowResize.Execute(a_Width, a_Height);
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		Application::Get()->Update();
		data.Draw();
		});
	
	glfwSetKeyCallback(m_Window, [](GLFWwindow* a_Window, int a_Key, int a_Keycode, int a_Action, int a_Modifier) {
		//This is data from the actual window, don't ask why you do this.
		WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		//Access the data via the UserPointer
		if(a_Action == GLFW_PRESS)
		{
			EventManager::GetInstance()->InputEvents.OnKeyPressed.Execute(a_Key);

		}
		if(a_Action == GLFW_RELEASE)
		{
			EventManager::GetInstance()->InputEvents.OnKeyReleased.Execute(a_Key);
		}
		if(a_Action == GLFW_REPEAT)
		{
			EventManager::GetInstance()->InputEvents.OnKeyRepeat.Execute(a_Key, 1);
		}
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* a_Window, int a_Button, int a_Action, int a_Mods) {
		if(a_Action == GLFW_PRESS)
		{
			EventManager::GetInstance()->InputEvents.OnMouseButtonPressed.Execute(a_Button);
		}
		if(a_Action == GLFW_RELEASE)
		{
			EventManager::GetInstance()->InputEvents.OnMouseButtonReleased.Execute(a_Button);
		}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
	
		EventManager::GetInstance()->InputEvents.OnMouseButtonScrolled.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
	});
	
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset) {
		EventManager::GetInstance()->InputEvents.OnMouseMoved.Execute(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
		});

	glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* a_Window, int a_Iconified) {
		EventManager::GetInstance()->WindowEvents.OnWindowMinimize.Execute(a_Iconified == GLFW_TRUE);
		});
	glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* a_Window, int a_Maximized) {
	
		EventManager::GetInstance()->WindowEvents.OnWindowMaximize.Execute(a_Maximized == GLFW_TRUE);
		});
	glfwSetWindowPosCallback(m_Window, [](GLFWwindow* a_Window, int xpos, int ypos)
		{
			WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
			Application::Get()->Update();
			data.Draw();
		});
	glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* a_Window, int focused)
		{
			WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
		});
	glfwSetWindowContentScaleCallback(m_Window, [](GLFWwindow* a_Window, float xscale, float yscale)
		{
			WindowsWindow& data = *(WindowsWindow*)glfwGetWindowUserPointer(a_Window);
			Application::Get()->Update();
			data.Draw();
		});
	glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* a_Window, int entered)
		{
			if(entered)
			{
				Input::SetCursorInWindow(true);
			}else
			{
				Input::SetCursorInWindow(false); 
			}
		});
}


WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{
	if(Input::GetKeyDown(Keys::CTRL))
	{
		SetFullscreen(true);
	}
	if(Input::GetKeyDown(Keys::TAB))
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
	EventManager::GetInstance()->GameLoopEvents.OnRender.Execute();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	EventManager::GetInstance()->GameLoopEvents.OnRenderTransparent.Execute();
	glDisable(GL_BLEND);
	// Swap buffers
	glfwSwapBuffers(m_Window);
}

bool WindowsWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}


void Trickster::WindowsWindow::SetKeyDown(int Key, bool a_bool)
{
	switch(Key)
	{
	case GLFW_KEY_Q:
		Input::SetKeyDown(Keys::Q, a_bool);
		break;
	case GLFW_KEY_W:
		Input::SetKeyDown(Keys::W, a_bool);
		break;
	case GLFW_KEY_E:
		Input::SetKeyDown(Keys::E, a_bool);
		break;
	case GLFW_KEY_R:
		Input::SetKeyDown(Keys::R, a_bool);
		break;
	case GLFW_KEY_T:
		Input::SetKeyDown(Keys::T, a_bool);
		break;
	case GLFW_KEY_Y:
		Input::SetKeyDown(Keys::Y, a_bool);
		break;
	case GLFW_KEY_U:
		Input::SetKeyDown(Keys::U, a_bool);
		break;
	case GLFW_KEY_I:
		Input::SetKeyDown(Keys::I, a_bool);
		break;
	case GLFW_KEY_O:
		Input::SetKeyDown(Keys::O, a_bool);
		break;
	case GLFW_KEY_P:
		Input::SetKeyDown(Keys::P, a_bool);
		break;
	case GLFW_KEY_A:
		Input::SetKeyDown(Keys::A, a_bool);
		break;
	case GLFW_KEY_S:
		Input::SetKeyDown(Keys::S, a_bool);
		break;
	case GLFW_KEY_D:
		Input::SetKeyDown(Keys::D, a_bool);
		break;
	case GLFW_KEY_F:
		Input::SetKeyDown(Keys::F, a_bool);
		break;
	case GLFW_KEY_G:
		Input::SetKeyDown(Keys::G, a_bool);
		break;
	case GLFW_KEY_H:
		Input::SetKeyDown(Keys::H, a_bool);
		break;
	case GLFW_KEY_J:
		Input::SetKeyDown(Keys::J, a_bool);
		break;
	case GLFW_KEY_K:
		Input::SetKeyDown(Keys::K, a_bool);
		break;
	case GLFW_KEY_L:
		Input::SetKeyDown(Keys::L, a_bool);
		break;
	case GLFW_KEY_Z:
		Input::SetKeyDown(Keys::Z, a_bool);
		break;
	case GLFW_KEY_X:
		Input::SetKeyDown(Keys::X, a_bool);
		break;
	case GLFW_KEY_C:
		Input::SetKeyDown(Keys::C, a_bool);
		break;
	case GLFW_KEY_V:
		Input::SetKeyDown(Keys::V, a_bool);
		break;
	case GLFW_KEY_B:
		Input::SetKeyDown(Keys::B, a_bool);
		break;
	case GLFW_KEY_N:
		Input::SetKeyDown(Keys::N, a_bool);
		break;
	case GLFW_KEY_M:
		Input::SetKeyDown(Keys::M, a_bool);
		break;
	case GLFW_KEY_TAB:
		Input::SetKeyDown(Keys::TAB, a_bool);
		break;
	case GLFW_KEY_CAPS_LOCK:
		Input::SetKeyDown(Keys::CAPSLOCK, a_bool);
		break;
	case GLFW_KEY_LEFT_SHIFT:
		Input::SetKeyDown(Keys::SHIFT, a_bool);
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		Input::SetKeyDown(Keys::SHIFT, a_bool);
		break;
	case GLFW_KEY_LEFT_CONTROL:
		Input::SetKeyDown(Keys::CTRL, a_bool);
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		Input::SetKeyDown(Keys::CTRL, a_bool);
		break;
	case GLFW_KEY_LEFT_ALT:
		Input::SetKeyDown(Keys::ALT, a_bool);
		break;
	case GLFW_KEY_RIGHT_ALT:
		Input::SetKeyDown(Keys::ALT, a_bool);
		break;
	case GLFW_KEY_SPACE:
		Input::SetKeyDown(Keys::SPACE, a_bool);
		break;
	case GLFW_KEY_1:
		Input::SetKeyDown(Keys::ONE, a_bool);
		break;
	case GLFW_KEY_2:
		Input::SetKeyDown(Keys::TWO, a_bool);
		break;
	case GLFW_KEY_3:
		Input::SetKeyDown(Keys::THREE, a_bool);
		break;
	case GLFW_KEY_4:
		Input::SetKeyDown(Keys::FOUR, a_bool);
		break;
	case GLFW_KEY_5:
		Input::SetKeyDown(Keys::FIVE, a_bool);
		break;
	case GLFW_KEY_6:
		Input::SetKeyDown(Keys::SIX, a_bool);
		break;
	case GLFW_KEY_7:
		Input::SetKeyDown(Keys::SEVEN, a_bool);
		break;
	case GLFW_KEY_8:
		Input::SetKeyDown(Keys::EIGHT, a_bool);
		break;
	case GLFW_KEY_9:
		Input::SetKeyDown(Keys::NINE, a_bool);
		break;
	case GLFW_KEY_0:
		Input::SetKeyDown(Keys::ZERO, a_bool);
		break;
	case GLFW_KEY_MINUS:
		Input::SetKeyDown(Keys::MINUS, a_bool);
		break;
	case GLFW_KEY_EQUAL:
		Input::SetKeyDown(Keys::EQUALS, a_bool);
		break;
	case GLFW_KEY_BACKSPACE:
		Input::SetKeyDown(Keys::BACKSPACE, a_bool);
		break;
	case GLFW_KEY_COMMA:
		Input::SetKeyDown(Keys::LESS_THAN, a_bool);
		break;
	case GLFW_KEY_PERIOD:
		Input::SetKeyDown(Keys::GREATER_THAN, a_bool);
		break;
	case GLFW_KEY_SLASH:
		Input::SetKeyDown(Keys::SLASH, a_bool);
		break;
	case GLFW_KEY_ESCAPE:
		Input::SetKeyDown(Keys::ESC, a_bool);
		break;
	case GLFW_KEY_ENTER:
		Input::SetKeyDown(Keys::ENTER, a_bool);
		break;
	
	}
}

void Trickster::WindowsWindow::SetFullscreen(bool a_Fullscreen)
{
	if (a_Fullscreen) {
		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, static_cast<int>(GetWidth()), static_cast<int>(GetHeight()), GLFW_DONT_CARE);
	}else
	{
		glfwSetWindowMonitor(m_Window, nullptr, m_PosX, m_PosY, GetWidth(), GetHeight(), GLFW_DONT_CARE);
		glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
	}
}


void Trickster::WindowsWindow::SetClick(int MouseKey, bool value)
{

	if (MouseKey >= 8)
	{
		LOG_ERROR("Not supposed to happen");
		return;
	}
	
	switch(MouseKey)
	{
	case GLFW_MOUSE_BUTTON_1:
		Input::SetClick(Mouse::Left, value);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		Input::SetClick(Mouse::Middle, value);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		Input::SetClick(Mouse::Right, value);
		break;
	case GLFW_MOUSE_BUTTON_4:
		Input::SetClick(Mouse::Four, value);
		break;
	case GLFW_MOUSE_BUTTON_5:
		Input::SetClick(Mouse::Five, value);
		break;
		
	}
}

void Trickster::WindowsWindow::Resize(int a_Width, int a_Height)
{
	m_Width = static_cast<unsigned int>(a_Width);
	m_Height = static_cast<unsigned int>(a_Height);
	//int left, top, right, bottom;
	//glfwGetWindowFrameSize(m_Window, &left, &top, &right, &bottom);
	glViewport(0, 0, m_Width, m_Height);
}
void* Trickster::WindowsWindow::GetRaw()
{
	return m_Window;
}
void Trickster::WindowsWindow::CaptureMouse(bool yoinkMouse)
{
	if (yoinkMouse) {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(m_Window, static_cast<double>(m_Width) / 2.0, static_cast<double>(m_Height) / 2.0);
	}
}
void WindowsWindow::Init(const WindowProps& props)
{
	m_Title = props.title;
	m_Width = props.width;
	m_Height = props.height;
	std::string output;
#ifdef DETAILED_CONSOLE
	output += "[Window] ";
#endif
	output += "Creating window " + props.title + ": " + std::to_string(props.width) + ", " + std::to_string(props.height) + ".";
	LOG(output);
	if(!s_GLFWInitialized)
	{
#ifdef DETAILED_CONSOLE
		LOG("[Window] Initializing GLFW.");
#endif
		int success = glfwInit();
		ASSERT(success);
		s_GLFWInitialized = true;
	}
	m_Window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), m_Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	//Loading in the icon
	int i_Width, i_Height, i_BPP;
	GLFWimage image[1];
	unsigned char* i_LocalBuffer = stbi_load("Resources/Icon/icon.png", &i_Width, &i_Height, &i_BPP, 0);
	image[0].height = i_Height;
	image[0].width = i_Width;
	image[0].pixels = i_LocalBuffer;
	int xpos, ypos, width, height;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
	glfwSetWindowPos(m_Window, width - m_Width, (height / 2 - (m_Height / 2)));
	glfwSetWindowIcon(m_Window, 1, image);
	glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);

#ifdef DETAILED_CONSOLE
	LOG("[Window] Initializing GLEW.");
#endif
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	SetVSync(true);
	//Listener to the windowclose.
	EventManager::GetInstance()->InputEvents.OnKeyPressed.AddListener(std::bind(&WindowsWindow::SetKeyDown, this, std::placeholders::_1, true));
	EventManager::GetInstance()->InputEvents.OnKeyReleased.AddListener(std::bind(&WindowsWindow::SetKeyDown, this, std::placeholders::_1, false));
	EventManager::GetInstance()->WindowEvents.OnWindowClose.AddListener(std::bind(&WindowsWindow::Shutdown, this));
	EventManager::GetInstance()->InputEvents.OnMouseButtonPressed.AddListener(std::bind(&WindowsWindow::SetClick, this, std::placeholders::_1, true));
	EventManager::GetInstance()->InputEvents.OnMouseButtonReleased.AddListener(std::bind(&WindowsWindow::SetClick, this, std::placeholders::_1, false));
	EventManager::GetInstance()->WindowEvents.OnWindowResize.AddListener(std::bind(&WindowsWindow::Resize, this, std::placeholders::_1, std::placeholders::_2));
}

void WindowsWindow::Shutdown()
{
	LOG("Shutting down.");
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void WindowsWindow::SetTitle(std::string a_Title)
{
	glfwSetWindowTitle(m_Window, a_Title.c_str());
}
