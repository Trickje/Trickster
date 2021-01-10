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
#include "Rendering/Shader.h"
#include "Rendering/ShaderManager.h"
#include "Rendering/TextureManager.h"
#include "Rendering/SpriteManager.h"
#include "Rendering/Texture.h"
#include "Rendering/Window.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/VertexArray.h"

namespace Trickster {
	DrawData2D::DrawData2D(void* data, int size)
	{
		vb = new VertexBuffer(data, size);
		va = new VertexArray();
		layout = new VertexBufferLayout();
	};
	
	Drawable2D::Drawable2D(const std::string& a_FilePath, const glm::vec2 a_Position, const glm::vec2 a_Scale)
		: m_Position(a_Position), m_Scale(a_Scale), m_FilePath(a_FilePath)
	{
		m_DrawData = new DrawData2D(
			&m_Vertices[0], sizeof(m_Vertices));
		GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic")->Get(), "ScreenPos"));
		GLCall(m_ScaleUniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic")->Get(), "Scale"));
		m_Size = { TextureManager::GetTexture(m_FilePath)->GetWidth(),  TextureManager::GetTexture(m_FilePath)->GetHeight() };
		m_DrawData->layout->Push<float>(2);
		m_DrawData->va->AddBuffer(*m_DrawData->vb, *m_DrawData->layout);
		m_Visible = true;
		if(TextureManager::GetTexture(m_FilePath)->GetBPP() == 4)
		{
			m_Transparent = true;
			EventManager::GetInstance()->GameLoopEvents.OnRenderTransparent.AddListener(std::bind(&Drawable2D::Draw, this));
		}
		else
		{
			m_Transparent = false;
			SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
		}
	}


	Drawable2D::~Drawable2D()
	{
	}

	void Drawable2D::Draw()
	{
		if (m_Visible) {
			ShaderManager::GetShader("basic")->Bind();
			glm::vec2 ScreenPos = ToScreenPos();
			glUniform2f(m_UniformLoc, ScreenPos.x, ScreenPos.y);
			const glm::vec2 l_Scale = TextureManager::GetTexture(m_FilePath)->GetScale();
			glUniform2f(m_ScaleUniformLoc, m_Scale.x * l_Scale.x, m_Scale.y * l_Scale.y);

			TextureManager::GetTexture(m_FilePath)->Bind();
			m_DrawData->vb->Bind();
			m_DrawData->va->Bind();
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}

	}

	void Drawable2D::SetScale(const float a_Width, const float a_Height)
	{
		m_Scale.x = a_Width;
		m_Scale.y = a_Height;
	}

	void Drawable2D::SetTexture(const std::string& a_FilePath)
	{
		m_FilePath = a_FilePath;
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
}
