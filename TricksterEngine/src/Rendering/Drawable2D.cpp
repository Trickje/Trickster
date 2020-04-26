#include "pch.h"

#include "Drawable2D.h"


#include "Engine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "SpriteManager.h"

namespace Trickster {
	Drawable2D::Drawable2D()
	{
		m_DrawData = new DrawData2D(
			&m_Vertices[0], sizeof(m_Vertices));
		GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic")->Get(), "ScreenPos"));
		m_FilePath = "../../TricksterEngine/TricksterEngine/Resources/image.png";
		m_DrawData->layout->Push<float>(2);
		m_DrawData->va->AddBuffer(*m_DrawData->vb, *m_DrawData->layout);
		m_Size = { TextureManager::GetTexture(m_FilePath)->GetWidth(),  TextureManager::GetTexture(m_FilePath)->GetHeight() };
		m_Position = { 0.f, 0.f };
		SpriteManager::GetInstance()->m_Drawable2Ds.push_back(this);
	}

	Drawable2D::Drawable2D(const glm::vec2 a_Position, const glm::vec2 a_Scale, const std::string& a_FilePath)
		: m_Position(a_Position), m_Scale(a_Scale), m_FilePath(a_FilePath)
	{
		m_DrawData = new DrawData2D(
			&m_Vertices[0], sizeof(m_Vertices));
		GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic")->Get(), "ScreenPos"));
		GLCall(m_ScaleUniformLoc = glGetUniformLocation(ShaderManager::GetShader("basic")->Get(), "Scale"));
		m_Size = { TextureManager::GetTexture(m_FilePath)->GetWidth(),  TextureManager::GetTexture(m_FilePath)->GetHeight() };
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
		ShaderManager::GetShader("basic")->Bind();
		glm::vec2 ScreenPos = ToScreenPos();
		glUniform2f(m_UniformLoc, ScreenPos.x, ScreenPos.y);
		//glUniform2f(m_UniformLoc, 0.f, 0.f);
		const glm::vec2 l_Scale = TextureManager::GetTexture(m_FilePath)->GetScale();
		glUniform2f(m_ScaleUniformLoc, m_Scale.x * l_Scale.x, m_Scale.y * l_Scale.y);

		TextureManager::GetTexture(m_FilePath)->Bind();
		m_DrawData->vb->Bind();
		m_DrawData->va->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	}

	void Drawable2D::SetScale(const float a_Width, const float a_Height)
	{
		m_Scale.x = a_Width;
		m_Scale.y = a_Height;
	}

	void Drawable2D::SetTexture(const std::string& a_FilePath)
	{
		m_FilePath = a_FilePath;
	}

	glm::vec2 Drawable2D::GetSize() const
	{
		return { m_Size.x * m_Scale.x, m_Size.y * m_Scale.y };
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

	glm::vec2 Drawable2D::ToScreenPos()
	{
		return {m_Position.x / (Application::Get()->GetWindow()->GetWidth() * 0.5f) - 1.f
			, m_Position.y / (Application::Get()->GetWindow()->GetHeight() * 0.5f) - 1.f};
	}
}
