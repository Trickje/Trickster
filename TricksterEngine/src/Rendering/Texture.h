#pragma once
namespace Trickster {
	class Texture
	{
	public:
		Texture(const std::string& a_FileName);
		~Texture();
		void Bind(unsigned int slot = 0) const;
		static void Unbind();
		int GetWidth() const;
		int GetHeight() const;
		int GetBPP() const;
		glm::vec2 GetScale() const;
	private:
		unsigned int m_RendererID;
		std::string m_FileName;
		unsigned char* m_LocalBuffer;
		int m_Width;
		int m_Height;
		int m_BPP;
	};

}