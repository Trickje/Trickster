#pragma once
#include <glm/vec2.hpp>

#include "Renderer.h"
namespace Trickster {
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();
		void Bind(unsigned int slot = 0) const;
		static void Unbind();
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	};

}