#include "pch.h"
#include "Input.h"
#include "Window.h"

using namespace Trickster;
Input* Input::instance = new Input();
bool Trickster::Input::GetKeyDownImpl(int Key)
{
	if (Key >= 54)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(Key + 1) + " out of 53.");
		return false;
	}
	return Keys[Key];
}

void Input::SetKeyDownImpl(int Key, bool a_bool)
{
	if(Key >= 54)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(Key + 1) + " out of 53.");
		return;
	}
	Keys[Key] = a_bool;
}
bool Trickster::Input::GetClickImpl(int MouseKey)
{
	if (MouseKey >= 8)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(MouseKey + 1) + " out of 53.");
		return false;
	}
	return MouseKeys[MouseKey];
}
void Trickster::Input::SetClickImpl(int MouseKey, bool a_bool)
{
	if (MouseKey >= 8)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(MouseKey + 1) + " out of 53.");
		return;
	}
	MouseKeys[MouseKey] = a_bool;
}

Input::Input() : Keys(), MouseKeys()
{
}

Trickster::Input::~Input()
{
}

bool Trickster::Input::GetKeyDown(int Key)
{
	return instance->GetKeyDownImpl(Key);
}
void Trickster::Input::SetKeyDown(int Key, bool a_bool)
{
	instance->SetKeyDownImpl(Key, a_bool);
}
bool Trickster::Input::GetClick(int MouseKey)
{
	return instance->GetClickImpl(MouseKey);
}
void Trickster::Input::SetClick(int MouseKey, bool a_bool)
{
	instance->SetClickImpl(MouseKey, a_bool);
}
glm::vec2 Trickster::Input::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetRaw()), &x, &y);
	return { x,y };
}