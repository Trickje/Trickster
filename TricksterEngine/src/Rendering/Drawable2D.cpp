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

#include "Rendering/Drawable2D.h"

#include "Core/Application.h"
#include "Events/EventManager.h"

using namespace TE;
	
	
Drawable2D::Drawable2D(const std::string& a_FilePath, const glm::vec2 a_Position, const glm::vec2 a_Scale)
	: m_Position(a_Position, 0.f), m_Scale(a_Scale), m_Name(a_FilePath)
{
	m_Size = {};
	m_Static = false;
	m_Visible = true;
	m_Transparent = true;
	EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Drawable2D::Draw, this));
	
}


Drawable2D::~Drawable2D()
{
}

void Drawable2D::Draw()
{
	//If the vertex buffer is not up to date
	//Generate a vertex buffer here
	//TODO: implement a draw function for this object in the renderer
	m_TranslationMatrix = glm::identity<glm::mat4>();
	m_RotationMatrix = glm::identity<glm::mat4>();
	m_ScaleMatrix = glm::identity<glm::mat4>();
	glm::scale(m_ScaleMatrix, {1920.f, 1080.f, 1.f});
	if(m_Visible)
		Application::Get()->m_RenderAPI->DrawSprite(m_Name, m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix, true);
}
	

void Drawable2D::SetScale(const float a_Width, const float a_Height)
{
	m_Scale.x = a_Width;
	m_Scale.y = a_Height;
}

void Drawable2D::SetTexture(const std::string& a_FilePath)
{
	m_Name = a_FilePath;
}

glm::vec2 Drawable2D::GetSize() const
{
	return { m_Size.x * m_Scale.x, m_Size.y * m_Scale.y };
}

void Drawable2D::SetPosition(float a_X, float a_Y)
{
	m_Position.x = a_X;
	m_Position.y = a_Y;
}

glm::vec2 Drawable2D::GetPosition() const
{
	return m_Position;
}

glm::vec2 Drawable2D::ToScreenPos()
{
	return {m_Position.x / (Application::Get()->GetWindow()->GetWidth() * 0.5f) - 1.f
		, m_Position.y / (Application::Get()->GetWindow()->GetHeight() * 0.5f) - 1.f};
}
void Drawable2D::SetVisible(bool isVisible)
{
	m_Visible = isVisible;
}
bool Drawable2D::IsVisible()
{
	return m_Visible;
}

