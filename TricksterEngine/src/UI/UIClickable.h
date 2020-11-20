#pragma once
#include <glm/vec2.hpp>

#include "ClickableBox.h"
#include "Rendering/Drawable2D.h"
namespace Trickster {
	class UIClickable : public ClickableBox
	{
	public:
		UIClickable();
		virtual ~UIClickable();
		UIClickable(const std::string& a_FilePath, const glm::vec2& a_Pos, const float& a_Width, const float& a_Height);

		UIClickable(float X, float Y, float a_Width, float a_Height);
		virtual void OnUpdate();
		virtual void OnClick();
		bool isClicked();
		virtual void SetPosition(const glm::vec2& a_Position);
	private:
		std::unique_ptr<Trickster::Drawable2D> m_Drawable;
	};
}