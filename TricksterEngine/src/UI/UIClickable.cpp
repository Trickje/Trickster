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
#include "UI/UIClickable.h"

#include "Events/EventManager.h"
#include "Rendering/Drawable2D.h"
using namespace TE;
UIClickable::~UIClickable()
{
	
}

UIClickable::UIClickable(const std::string& a_FilePath, const glm::vec2& a_Pos, const float& a_Width, const float& a_Height)
	: ClickableBox(a_Pos.x, a_Pos.y, a_Width, a_Height)
{
	m_Drawable = std::make_unique<Drawable2D>(a_FilePath, a_Pos);
	glm::vec2 size = m_Drawable->GetSize();
	m_Drawable->SetScale(a_Width / size.x, a_Height / size.y);
	
}

UIClickable::UIClickable(float X, float Y, float a_Width, float a_Height): ClickableBox(X, Y, a_Width, a_Height)
{
	
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&UIClickable::OnUpdate, this));
}

void UIClickable::OnUpdate()
{
	ClickableBox::OnUpdate();
}

void UIClickable::OnClick()
{
	ClickableBox::OnClick();
}

bool UIClickable::isClicked()
{
	return m_Clicked;
}

void UIClickable::SetPosition(const glm::vec2& a_Position)
{
	ClickableBox::SetPosition(a_Position);
	m_Drawable->SetPosition(a_Position.x, a_Position.y);
}

void UIClickable::SetVisible(bool isVisible)
{
	m_Drawable->SetVisible(isVisible);
}
