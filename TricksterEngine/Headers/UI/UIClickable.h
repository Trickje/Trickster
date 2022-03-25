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
#include "ClickableBox.h"
namespace TE {
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
		std::unique_ptr<Drawable2D> m_Drawable;
	};
}