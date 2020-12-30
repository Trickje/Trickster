#pragma once
namespace Trickster {
	class Texture;

	class TextureManager
	{
	public:
		static Texture* GetTexture(const std::string a_TextureName);
		static TextureManager* GetInstance();
	private:
		static TextureManager* instance;
		std::map<std::string, Texture*> map;
		TextureManager();
		~TextureManager();
	};

}