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
namespace Trickster {
	
	class Drawable2D 
	{
	public:
		TRICKSTER_API Drawable2D(const std::string& a_FilePath, const glm::vec2 a_Position, const glm::vec2 a_Scale);
		TRICKSTER_API virtual ~Drawable2D();
		TRICKSTER_API void Draw();

		TRICKSTER_API void SetScale(const float a_Width, const float a_Height);

		TRICKSTER_API void SetTexture(const std::string& a_FilePath);

		TRICKSTER_API glm::vec2 GetSize() const;
		
		TRICKSTER_API void SetPosition(float a_X, float a_Y);

		TRICKSTER_API glm::vec2 GetPosition() const;

		TRICKSTER_API glm::vec2 ToScreenPos();
		
		TRICKSTER_API void SetVisible(bool isVisible);
		
		TRICKSTER_API bool IsVisible();
	private:
		glm::vec2 m_Scale;
		glm::vec2 m_Size;
		std::string m_Name;

		bool m_Visible;
		bool m_Transparent;
		bool m_Static;

		glm::mat4 m_ModelMatrix = {};
		glm::mat4 m_RotationMatrix = {};
		glm::mat4 m_ScaleMatrix = {};
		float m_Yaw = 0.f;
		float m_Pitch = 0.f;
		float m_Roll = 0.f;
		glm::vec3 m_Position = {};
		glm::mat4 m_TranslationMatrix = {};
	};


}