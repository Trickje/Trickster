#pragma once
#include "ClickableBox.h"
namespace Trickster {
	class Drawable2D;
	class UIClickable : public ClickableBox
	{
	public:
		TRICKSTER_API UIClickable();
		TRICKSTER_API virtual ~UIClickable();
		TRICKSTER_API UIClickable(const std::string& a_FilePath, const glm::vec2& a_Pos, const float& a_Width, const float& a_Height);

		TRICKSTER_API UIClickable(float X, float Y, float a_Width, float a_Height);
		TRICKSTER_API virtual void OnUpdate();
		TRICKSTER_API virtual void OnClick();
		TRICKSTER_API bool isClicked();
		TRICKSTER_API virtual void SetPosition(const glm::vec2& a_Position);
		TRICKSTER_API void SetVisible(bool isVisible);
	private:
		std::unique_ptr<Trickster::Drawable2D> m_Drawable;
	};
}