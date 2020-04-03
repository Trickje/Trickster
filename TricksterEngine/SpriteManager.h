#pragma once
#include <vector>

class Drawable2D;
class SpriteManager
{
public:
	static SpriteManager* GetInstance();
	std::vector<Drawable2D*> m_Drawable2Ds;
	std::vector<std::string> m_TexturePaths;
private:
	static SpriteManager* instance;
	SpriteManager();
	~SpriteManager();
};
