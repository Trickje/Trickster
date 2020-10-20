#include "pch.h"
#include "ClickableBox.h"

#include "Application.h"
#include <Logger.h>
ClickableBox::ClickableBox(float X, float Y, float a_Width, float a_Height)
{
	this->m_Pos = { X,Y };
	m_Width = a_Width;
	m_Height = a_Height;
	Trickster::EventManager::GetInstance()->OnUpdate.AddListener(std::bind(&ClickableBox::OnUpdate, this));
}

ClickableBox::~ClickableBox()
{
	
}

void ClickableBox::OnUpdate()
{
	if (IsHovered() && Trickster::Application::Get()->GetWindow()->GetClick(Trickster::Mouse::Left) && !IsAlreadyClicked && m_Clickable)
	{
		IsAlreadyClicked = true;
		LOG("Clicked");
	}
	if(!Trickster::Application::Get()->GetWindow()->GetClick(Trickster::Mouse::Left))
	{
		IsAlreadyClicked = false;
	}
}

bool ClickableBox::IsHovered()
{
	double x, y;
	Trickster::Application::Get()->GetWindow()->GetCursorPos(&x, &y);
	if(WidthContains(x) && HeightContains(y))
	{
		return true;
	}
	return false;
}

void ClickableBox::SetPosition(glm::vec2 a_Vec)
{
	m_Pos = a_Vec;
}

glm::vec2 ClickableBox::GetPosition() const
{
	return m_Pos;
}

void ClickableBox::SetWidth(float a_Width)
{
	m_Width = a_Width;
}

float ClickableBox::GetWidth() const
{
	return m_Width;
}

void ClickableBox::SetHeight(float a_Height)
{
	m_Height = a_Height;
}

float ClickableBox::GetHeight() const
{
	return m_Height;
}

void ClickableBox::SetClickable(bool a_Value)
{
	m_Clickable = a_Value;
}

bool ClickableBox::WidthContains(double x)
{
	
	return ((m_Pos.x + m_Width) > x && x > m_Pos.x);
}

bool ClickableBox::HeightContains(double y)
{
	return ((m_Pos.y+ m_Height) > y && y > m_Pos.y);
}
