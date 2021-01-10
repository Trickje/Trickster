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
	class VertexBufferLayout;
	class VertexArray;
	class VertexBuffer;
	class Drawable;
	struct DrawData2D
	{
		DrawData2D(void* data, int size);
		VertexBuffer* vb;
		VertexArray* va;
		VertexBufferLayout* layout;
	};

	class Drawable2D 
	{
	public:
		Drawable2D(const std::string& a_FilePath, const glm::vec2 a_Position = { 0.f,0.f }, const glm::vec2 a_Scale = { 1.f, 1.f });
		virtual ~Drawable2D();
		virtual void Draw();
		void SetScale(const float a_Width, const float a_Height);
		void SetTexture(const std::string& a_FilePath);
		glm::vec2 GetSize() const;
		void SetPosition(const float a_X, const float a_Y);
		glm::vec2 GetPosition() const;
		glm::vec2 ToScreenPos();
		void SetVisible(bool isVisible = true);
		bool IsVisible();
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		//Texture file path
		std::string m_FilePath;
		DrawData2D* m_DrawData;

		glm::vec2 m_Scale;
		//Location of the ScreenPos uniform in the shader
		GLint m_UniformLoc;
		//Location of the Scale uniform in the shader
		GLint m_ScaleUniformLoc;
		bool m_Transparent;
		
	private:
		float m_Vertices[12] =
		{
			0.f, 0.f,	//Bottom left
			0.f, 1.f,	//Top left
			1.f, 1.f,	//Top right
			1.f, 1.f,	//Top right
			1.f, 0.f,	//Bottom right
			0.f, 0.f	//Bottom left
		};
		bool m_Visible;
	};


}