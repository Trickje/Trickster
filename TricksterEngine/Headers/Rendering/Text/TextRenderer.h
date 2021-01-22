#pragma once
#include "Internal/Font.h"
namespace Trickster
{
	struct TextRenderQueueObject
	{
		TextRenderQueueObject(std::string str, glm::vec2 pos, float scale, glm::vec4 color, std::shared_ptr<Font> font)
		{
			String = str;
			Position = pos;
			Scale = scale;
			Color = color;
			Font = font;
		}
		std::string String;
		glm::vec2 Position;
		float Scale;
		glm::vec4 Color;
		std::shared_ptr<Font> Font;
	};
	class TextRenderer
	{
	public:
		TRICKSTER_API TextRenderer(const TextRenderer&) = delete;
		TRICKSTER_API TextRenderer& operator=(TextRenderer other) = delete;
		TRICKSTER_API static TextRenderer* Get();
		TRICKSTER_API void Initialize();
		//Adds a string to the render queue
		TRICKSTER_API void RenderString(const std::string& a_String, glm::vec2 a_Position, float a_Scale = 1.f, glm::vec4 a_Color = {1.f,1.f,1.f, 1.f}, const std::string& a_FontName = Get()->DefaultFont);

		TRICKSTER_API void FlushQueue();
		TRICKSTER_API ~TextRenderer();
	private:
		static TextRenderer* m_Instance;
		TRICKSTER_API TextRenderer();
		TRICKSTER_API void LoadFont(std::string a_FontName);
		std::string DefaultFont = "arial";
		std::map<std::string, std::shared_ptr<Font>> m_Fonts;
		std::vector<std::shared_ptr<Font>> m_RenderQueue;
	};
}
