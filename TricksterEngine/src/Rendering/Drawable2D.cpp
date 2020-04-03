
#include "Drawable2D.h"


#include "Engine.h"
#include "Logger.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"

namespace Trickster {
	Drawable2D::Drawable2D()
	{
		m_DrawData = new DrawData2D(
			&m_Vertices[0], sizeof(m_Vertices));
		GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic.shader")->Get(), "ScreenPos"));
		m_FilePath = "Resources/image.png";
		m_DrawData->layout->Push<float>(2);
		m_DrawData->va->AddBuffer(*m_DrawData->vb, *m_DrawData->layout);
		m_Size = { 0.f, 0.f };
		m_Position = { 0.f, 0.f };
		SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
	}

	Drawable2D::Drawable2D(const glm::vec2 a_Position, const glm::vec2 a_Size, const std::string& a_FilePath)
		: m_Position(a_Position), m_Size(a_Size), m_FilePath(a_FilePath)
	{
		m_DrawData = new DrawData2D(
			&m_Vertices[0], sizeof(m_Vertices));
		GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic.shader")->Get(), "ScreenPos"));
		m_DrawData->layout->Push<float>(2);
		m_DrawData->va->AddBuffer(*m_DrawData->vb, *m_DrawData->layout);
		SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
	}


	Drawable2D::~Drawable2D()
	{
	}

	void Drawable2D::Draw()
	{
		//	LOG("draw");
		ShaderManager::GetShader("basic.shader")->Bind();
		//glUniform2f(m_UniformLoc, m_Position.x, m_Position.y);
		glUniform2f(m_UniformLoc, 0.f, 0.f);

		TextureManager::GetTexture(m_FilePath)->Bind();
		m_DrawData->vb->Bind();
		m_DrawData->va->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	}

	void Drawable2D::SetSize(const float a_Width, const float a_Height)
	{
		m_Size.x = a_Width;
		m_Size.y = a_Height;
	}

	void Drawable2D::SetTexture(const std::string& a_FilePath)
	{
		m_FilePath = a_FilePath;
	}

	glm::vec2 Drawable2D::GetSize() const
	{
		return m_Size;
	}

	void Drawable2D::SetPosition(float a_X, float a_Y)
	{
		m_Position.x = a_X;
		m_Position.y = a_Y;
	}

	glm::vec2 Drawable2D::GetPosition() const
	{
		return m_Position;
	}
}