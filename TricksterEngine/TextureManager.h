#pragma once
#include <map>
#include <string>
namespace Trickster {
	class Texture;

	class TextureManager
	{
	public:
		static Texture* GetTexture(const std::string texturePath);
		static TextureManager* GetInstance();
	private:
		static TextureManager* instance;
		std::map<std::string, Texture*> map;
		TextureManager();
		~TextureManager();
	};

}