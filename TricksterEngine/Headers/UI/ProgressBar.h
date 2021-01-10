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
	class VertexBuffer;
	class VertexBufferLayout;
	class VertexArray;
	class ProgressBar
	{
	public:
		ProgressBar();
		~ProgressBar();
		TRICKSTER_API void SetPercentage(float a_Percentage);
		TRICKSTER_API float GetPercentage();
		TRICKSTER_API void Draw();
		//A percentage of the screen, 0-100
		TRICKSTER_API void SetPosition(const glm::vec2& a_Position);
		TRICKSTER_API const glm::vec2& GetPosition();
		TRICKSTER_API void SetSize(const glm::vec2& a_Size);
		TRICKSTER_API const glm::vec2& GetSize();
		TRICKSTER_API void SetScale(const glm::vec2& a_Scale);
		TRICKSTER_API const glm::vec2& GetScale();
		TRICKSTER_API void SetColorFront(const glm::vec3& a_Color);
		TRICKSTER_API const glm::vec3& GetColorFront();
		TRICKSTER_API void SetColorBack(const glm::vec3& a_Color);
		TRICKSTER_API const glm::vec3& GetColorBack();
		
	private:
		TRICKSTER_API void FillVertices();
		glm::vec2 ToScreenPos();
		glm::vec2 m_Position;
		glm::vec3 m_ColorFront;
		glm::vec3 m_ColorBack;
		glm::vec2 m_Size;
		//Texture file path
		std::string m_FilePath;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBufferLayout> layout;
		float m_Percentage;
		glm::vec2 m_Scale;
		//Location of the ScreenPos uniform in the shader
		GLint m_UniformLoc;
		//Location of the Scale uniform in the shader
		GLint m_ScaleUniformLoc;
		float m_Vertices[60] =
		{
			0.f, 0.f,	 1.f, 1.f, 1.f,//Bottom left
			0.f, 1.f,	 1.f, 1.f, 1.f,//Top left
			1.f, 1.f,	 1.f, 1.f, 1.f,//Top right
			1.f, 1.f,	 1.f, 1.f, 1.f,//Top right
			1.f, 0.f,	 1.f, 1.f, 1.f,//Bottom right
			0.f, 0.f,	 1.f, 1.f, 1.f,//Bottom left
			
			0.f, 0.f,	0.f, 0.f, 0.f,//Bottom left
			0.f, 1.f,	0.f, 0.f, 0.f,//Top left
			1.f, 1.f,	0.f, 0.f, 0.f,//Top right
			1.f, 1.f,	0.f, 0.f, 0.f,//Top right
			1.f, 0.f,	0.f, 0.f, 0.f,//Bottom right
			0.f, 0.f,	0.f, 0.f, 0.f//Bottom left
			
			
		};
	};
}
