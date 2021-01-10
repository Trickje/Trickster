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
#include "UI/ClickableBox.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Events/EventManager.h"
#include "Rendering/Window.h"

ClickableBox::ClickableBox(float X, float Y, float a_Width, float a_Height)
{
	this->m_Pos = { X,Y };
	m_Width = a_Width;
	m_Height = a_Height;
	Trickster::EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&ClickableBox::OnUpdate, this));
}

ClickableBox::~ClickableBox()
{
	
}

void ClickableBox::OnUpdate()
{
	m_Clicked = false;
	if (IsHovered() && Trickster::Input::GetClick(Trickster::Mouse::Left) && !IsAlreadyClicked && !AwaitingClick && m_Clickable)
	{
		if(Trickster::Application::Get()->IsTickBased())
		{
			IsAlreadyClicked = true;
			AwaitingClick = true;
			Trickster::EventManager::GetInstance()->GameLoopEvents.TickOnce.AddListener(std::bind(&ClickableBox::OnClick, this));
		}
		else {
			OnClick();
		}
	}
	if(!Trickster::Input::GetClick(Trickster::Mouse::Left))
	{
		IsAlreadyClicked = false;
	}
}

bool ClickableBox::IsHovered()
{
	glm::vec2 mousePos = Trickster::Input::GetMousePos();
	
	if(WidthContains(mousePos.x) && HeightContains(static_cast<float>(Trickster::Application::Get()->GetWindow()->GetHeight()) - mousePos.y))
	{
		return true;
	}
	return false;
}

void ClickableBox::SetPosition(glm::vec2 a_Vec)
{
	m_Pos = a_Vec;
}

glm::vec2 ClickableBox::GetPosition() const
{
	return m_Pos;
}

void ClickableBox::SetWidth(float a_Width)
{
	m_Width = a_Width;
}

float ClickableBox::GetWidth() const
{
	return m_Width;
}

void ClickableBox::SetHeight(float a_Height)
{
	m_Height = a_Height;
}

float ClickableBox::GetHeight() const
{
	return m_Height;
}

void ClickableBox::SetClickable(bool a_Value)
{
	m_Clickable = a_Value;
}

bool ClickableBox::WidthContains(double x)
{
	
	return ((static_cast<double>(m_Pos.x) + static_cast<double>(m_Width)) > x && x > static_cast<double>(m_Pos.x));
}

bool ClickableBox::HeightContains(double y)
{
	return ((static_cast<double>(m_Pos.y)+ static_cast<double>(m_Height)) > y && y > static_cast<double>(m_Pos.y));
}

void ClickableBox::OnClick()
{
	m_Clicked = true;


	if(Trickster::Application::Get()->IsTickBased())
	{
		AwaitingClick = false;
	}
}