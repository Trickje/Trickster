#include "pch.h"
#include "UIClickable.h"
using namespace Trickster;
UIClickable::~UIClickable()
{
	
}

UIClickable::UIClickable(const std::string& a_FilePath, const glm::vec2& a_Pos, const float& a_Width, const float& a_Height)
	: ClickableBox(a_Pos.x, a_Pos.y, a_Width, a_Height)
{
	m_Drawable = std::make_unique<Trickster::Drawable2D>(a_Pos, glm::vec2(1,1), a_FilePath);
	glm::vec2 size = m_Drawable->GetSize();
	m_Drawable->SetScale(a_Width / size.x, a_Height / size.y);
	
}

UIClickable::UIClickable(float X, float Y, float a_Width, float a_Height): ClickableBox(X, Y, a_Width, a_Height)
{
	
	Trickster::EventManager::GetInstance()->OnUpdate.AddListener(std::bind(&UIClickable::OnUpdate, this));
}

void UIClickable::OnUpdate()
{
	ClickableBox::OnUpdate();
}

void UIClickable::OnClick()
{
	ClickableBox::OnClick();
	LOG("Click");
}

bool Trickster::UIClickable::isClicked()
{
	return m_Clicked;
}

void UIClickable::SetPosition(const glm::vec2& a_Position)
{
	ClickableBox::SetPosition(a_Position);
	m_Drawable->SetPosition(a_Position.x, a_Position.y);
}
