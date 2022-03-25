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
#include "Core/Input.h"
#include "Rendering/Window.h"
#include "Core/Application.h"

using namespace TE;
Input* Input::instance = new Input();
bool Input::GetKeyDownImpl(int Key)
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
bool Input::GetClickImpl(int MouseKey)
{
	if (MouseKey >= 8)
	{
		LOG_ERROR("Trying to get a key out of bounds, " + std::to_string(MouseKey + 1) + " out of 53.");
		return false;
	}
	return MouseKeys[MouseKey];
}
void Input::SetClickImpl(int MouseKey, bool a_bool)
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
	CursorInWindow = true;
}

Input::~Input()
{
}

bool Input::GetKeyDown(int Key)
{
	return instance->GetKeyDownImpl(Key);
}
void Input::SetKeyDown(int Key, bool a_bool)
{
	instance->SetKeyDownImpl(Key, a_bool);
}
bool Input::GetClick(int MouseKey)
{
	if (instance->CursorInWindow) {
		return instance->GetClickImpl(MouseKey);
	}else
	{
		return false;
	}
}
void Input::SetClick(int MouseKey, bool a_bool)
{
	if (instance->CursorInWindow) {
		instance->SetClickImpl(MouseKey, a_bool);
	}else
	{
		return;
	}
}
glm::vec2 Input::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetRaw()), &x, &y);
	return { x,y };
}

void Input::SetCursorInWindow(bool isInWindow)
{
	instance->CursorInWindow = isInWindow;
}
