#pragma once
#include <glm/vec2.hpp>

#include "Application.h"
#include "Engine.h"
#include "Renderer.h"
namespace Trickster {
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();
		void Bind(unsigned int slot = 0) const;
		static void Unbind();
		int GetWidth() const;
		int GetHeight() const;
		glm::vec2 GetScale() const { return { m_Width / (Application::Get()->GetWindow()->GetWidth() * 0.5f), m_Height / (Application::Get()->GetWindow()->GetHeight() * 0.5f) }; }
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width;
		int m_Height;
		int m_BPP;
	};

}