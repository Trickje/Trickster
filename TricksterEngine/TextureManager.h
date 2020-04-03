#pragma once
#include <map>
#include <string>

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

