#pragma once
/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
class ClickableBox
{
public:
	TRICKSTER_API ClickableBox(float X, float Y,float a_Width, float a_Height);
	TRICKSTER_API virtual ~ClickableBox();
	TRICKSTER_API virtual void OnUpdate();
	TRICKSTER_API bool IsHovered();
	TRICKSTER_API void SetPosition(glm::vec2 a_Vec);
	TRICKSTER_API glm::vec2 GetPosition()const;
	TRICKSTER_API void SetWidth(float a_Width);
	TRICKSTER_API float GetWidth()const;
	TRICKSTER_API void SetHeight(float a_Height);
	TRICKSTER_API float GetHeight()const;
	TRICKSTER_API void SetClickable(bool a_Value);
	TRICKSTER_API virtual void OnClick();
protected:
	TRICKSTER_API bool WidthContains(double x);
	TRICKSTER_API bool HeightContains(double y);
	glm::vec2 m_Pos;
	float m_Width, m_Height;
	bool IsAlreadyClicked = false;
	bool m_Clickable = true;
	bool AwaitingClick = false;
	bool m_Clicked = false;
};

