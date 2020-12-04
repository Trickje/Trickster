#include "pch.h"
#include "ProgressBar.h"
#include "ShaderManager.h"
#include "Shader.h"

#include <Rendering/VertexBuffer.h>
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBufferLayout.h"
#include "Window.h"

using namespace Trickster;

ProgressBar::ProgressBar() : m_Position(), m_ColorFront(), m_ColorBack(), m_Size(),  m_Percentage(), m_Scale()
{
	GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basicColor")->Get(), "ScreenPos"));
	GLCall(m_ScaleUniformLoc = glGetUniformLocation(ShaderManager::GetShader("basicColor")->Get(), "Scale"));

	vb = std::make_unique<VertexBuffer>(&m_Vertices[0], static_cast<unsigned int>(sizeof(m_Vertices)));
	va = std::make_unique<VertexArray>();
	layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(2);
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);
	m_Scale = { 1.f, 1.f };
	m_Size = { 90.f, 10.f };
	m_ColorFront = {0.f, 1.f, 0.f};
	m_ColorBack = { 0.7f, 0.f, 0.f };
	EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&ProgressBar::Draw, this));
	FillVertices();
}

ProgressBar::~ProgressBar()
{
	
}

void ProgressBar::SetPercentage(float a_Percentage)
{
	m_Percentage = glm::clamp(a_Percentage, 0.f, 1.f);

	FillVertices();
}

float Trickster::ProgressBar::GetPercentage()
{
	return m_Percentage;
}

void ProgressBar::Draw()
{
	ShaderManager::GetShader("basicColor")->Bind();
	const glm::vec2 ScreenPos = ToScreenPos();
	glUniform2f(m_UniformLoc, ScreenPos.x, ScreenPos.y);

	//glUniform2f(m_UniformLoc, 0.f, 0.f);
	//glUniform2f(m_ScaleUniformLoc, m_Scale.x, m_Scale.y);
	glUniform2f(m_ScaleUniformLoc, 1.f, 1.f);
	vb->Bind();
	va->AddBuffer(*vb, *layout);
	va->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 12));
}
void Trickster::ProgressBar::SetPosition(const glm::vec2& a_Position)
{
	m_Position = a_Position;
	FillVertices();
}
const glm::vec2& Trickster::ProgressBar::GetPosition()
{
	return m_Position;
}
void Trickster::ProgressBar::SetSize(const glm::vec2& a_Size)
{
	m_Size = a_Size;
	FillVertices();
}
const glm::vec2& Trickster::ProgressBar::GetSize()
{
	return m_Size;
}
void Trickster::ProgressBar::SetScale(const glm::vec2& a_Scale)
{
	m_Scale = a_Scale;
	FillVertices();
}
const glm::vec2& Trickster::ProgressBar::GetScale()
{
	return m_Scale;
}
void Trickster::ProgressBar::SetColorFront(const glm::vec3& a_Color)
{
	m_ColorFront = a_Color;
	FillVertices();
	
}
const glm::vec3& Trickster::ProgressBar::GetColorFront()
{
	return m_ColorFront;
}
void Trickster::ProgressBar::SetColorBack(const glm::vec3& a_Color)
{
	m_ColorBack = a_Color;
	FillVertices();
	
}
const glm::vec3& Trickster::ProgressBar::GetColorBack()
{
	return m_ColorBack;
}
void Trickster::ProgressBar::FillVertices()
{
	m_Vertices[0] = (2.f * m_Position.x / 100.f);
	m_Vertices[1] = (2.f * m_Position.y / 100.f);
	m_Vertices[2] = m_ColorFront.r;
	m_Vertices[3] = m_ColorFront.g;
	m_Vertices[4] = m_ColorFront.b;
	m_Vertices[5] = (2.f * m_Position.x / 100.f);
	m_Vertices[6] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[7] = m_ColorFront.r;
	m_Vertices[8] = m_ColorFront.g;
	m_Vertices[9] = m_ColorFront.b;
	m_Vertices[10] = (m_Percentage * 2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[11] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[12] = m_ColorFront.r;
	m_Vertices[13] = m_ColorFront.g;
	m_Vertices[14] = m_ColorFront.b;
	m_Vertices[15] = (m_Percentage * 2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[16] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[17] = m_ColorFront.r;
	m_Vertices[18] = m_ColorFront.g;
	m_Vertices[19] = m_ColorFront.b;
	m_Vertices[20] = (m_Percentage * 2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[21] = (2.f * m_Position.y / 100.f);
	m_Vertices[22] = m_ColorFront.r;
	m_Vertices[23] = m_ColorFront.g;
	m_Vertices[24] = m_ColorFront.b;
	m_Vertices[25] = (2.f * m_Position.x / 100.f);
	m_Vertices[26] = (2.f * m_Position.y / 100.f);
	m_Vertices[27] = m_ColorFront.r;
	m_Vertices[28] = m_ColorFront.g;
	m_Vertices[29] = m_ColorFront.b;
	m_Vertices[30] = (2.f * m_Position.x / 100.f);
	m_Vertices[31] = (2.f * m_Position.y / 100.f);
	m_Vertices[32] = m_ColorBack.r;
	m_Vertices[33] = m_ColorBack.g;
	m_Vertices[34] = m_ColorBack.b;
	m_Vertices[35] = (2.f * m_Position.x / 100.f);
	m_Vertices[36] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[37] = m_ColorBack.r;
	m_Vertices[38] = m_ColorBack.g;
	m_Vertices[39] = m_ColorBack.b;
	m_Vertices[40] = (2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[41] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[42] = m_ColorBack.r;
	m_Vertices[43] = m_ColorBack.g;
	m_Vertices[44] = m_ColorBack.b;
	m_Vertices[45] = (2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[46] = (2.f * m_Size.y) / 100.f + (2.f * m_Position.y / 100.f);
	m_Vertices[47] = m_ColorBack.r;
	m_Vertices[48] = m_ColorBack.g;
	m_Vertices[49] = m_ColorBack.b;
	m_Vertices[50] = (2.f * m_Size.x) / 100.f + (2.f * m_Position.x / 100.f);
	m_Vertices[51] = (2.f * m_Position.y / 100.f);
	m_Vertices[52] = m_ColorBack.r;
	m_Vertices[53] = m_ColorBack.g;
	m_Vertices[54] = m_ColorBack.b;
	m_Vertices[55] = (2.f * m_Position.x / 100.f);
	m_Vertices[56] = (2.f * m_Position.y / 100.f);
	m_Vertices[57] = m_ColorBack.r;
	m_Vertices[58] = m_ColorBack.g;
	m_Vertices[59] = m_ColorBack.b;

	std::vector<float> vertices;
	for (int i = 0; i < 60; i++)
	{
		vertices.push_back(m_Vertices[i]);
	}
	vb->ChangeData(vertices);
}
glm::vec2 ProgressBar::ToScreenPos()
{
	return { m_Position.x / (static_cast<float>(Application::Get()->GetWindow()->GetWidth()) * 0.5f) - 1.f
		, m_Position.y / (static_cast<float>(Application::Get()->GetWindow()->GetHeight()) * 0.5f) - 1.f };
}