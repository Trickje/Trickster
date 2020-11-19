#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <Rendering/VertexBuffer.h>


#include "Rendering/VertexArray.h"
#include "Rendering/VertexBufferLayout.h"

namespace Trickster {
	class ProgressBar
	{
	public:
		ProgressBar();
		~ProgressBar();
		void SetPercentage(float a_Percentage);
		float GetPercentage();
		void Draw();
		//A percentage of the screen, 0-100
		void SetPosition(const glm::vec2& a_Position);
		const glm::vec2& GetPosition();
		void SetSize(const glm::vec2& a_Size);
		const glm::vec2& GetSize();
		void SetScale(const glm::vec2& a_Scale);
		const glm::vec2& GetScale();
		void SetColorFront(const glm::vec3& a_Color);
		const glm::vec3& GetColorFront();
		void SetColorBack(const glm::vec3& a_Color);
		const glm::vec3& GetColorBack();
		
	private:
		void FillVertices();
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
