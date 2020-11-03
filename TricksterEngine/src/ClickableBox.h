#pragma once
#include <glm/vec2.hpp>

class ClickableBox
{
public:
	ClickableBox(float X, float Y,float a_Width, float a_Height);
	virtual ~ClickableBox();
	virtual void OnUpdate();
	bool IsHovered();
	void SetPosition(glm::vec2 a_Vec);
	glm::vec2 GetPosition()const;
	void SetWidth(float a_Width);
	float GetWidth()const;
	void SetHeight(float a_Height);
	float GetHeight()const;
	void SetClickable(bool a_Value);
	virtual void OnClick();
protected:
	bool WidthContains(double x);
	bool HeightContains(double y);
	glm::vec2 m_Pos;
	float m_Width, m_Height;
	bool IsAlreadyClicked = false;
	bool m_Clickable = true;
	bool AwaitingClick = false;
};

