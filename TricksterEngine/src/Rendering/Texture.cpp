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
#define STB_IMAGE_IMPLEMENTATION
#include "Rendering/Texture.h"
#include "Core/Application.h"
#include "Rendering/Window.h"
#include "stb_image.h"
namespace Trickster {
	Texture::Texture(const std::string& a_FileName)
		: m_FileName(a_FileName), m_LocalBuffer(nullptr),
		m_Width(0), m_Height(0), m_BPP(0)
	{
#ifdef TRICKSTER_OPENGL
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load( (Application::Get()->TexturePath + m_FileName).c_str(), &m_Width, &m_Height, &m_BPP, 0);
		if(m_Width == 0)
		{
			LOG_ERROR("[Texture] Failed to load " + m_FileName + "!");
		}
		//Generating texture holder
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		//MUST DEFINE THESE VARIABLES
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		//Assigning the data to opengl
		if (m_BPP == 4) {
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

		}else
		{
		
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
		}
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
		{
			stbi_image_free(m_LocalBuffer);
		}

		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
#endif
	}


	Texture::~Texture()
	{
#ifdef TRICKSTER_OPENGL
		GLCall(glDeleteTextures(1, &m_RendererID));
#endif
	}

	void Texture::Bind(unsigned slot) const
	{
#ifdef TRICKSTER_OPENGL
		//This is the texture slot
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
#endif
	}

	void Texture::Unbind()
	{
#ifdef TRICKSTER_OPENGL
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
#endif
	}

	int Texture::GetWidth() const
	{
		m_Width;
		return m_Width;
	}

	int Texture::GetHeight() const
	{
		m_Height;
		return m_Height;
	}
	int Texture::GetBPP() const
	{
		return m_BPP;
	}
	glm::vec2 Texture::GetScale() const
	{
		return	{ m_Width / (Application::Get()->GetWindow()->GetWidth() * 0.5f), m_Height / (Application::Get()->GetWindow()->GetHeight() * 0.5f) };
	}

}
