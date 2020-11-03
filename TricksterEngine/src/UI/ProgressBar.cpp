#include "pch.h"
#include "ProgressBar.h"
#include <GL/GL.h>



#include "Application.h"
#include "ShaderManager.h"
#include "Shader.h"
using namespace Trickster;

ProgressBar::ProgressBar()
{
	GLCall(m_UniformLoc = glGetUniformLocation(ShaderManager::GetShader("basicColor")->Get(), "ScreenPos"));
	GLCall(m_ScaleUniformLoc = glGetUniformLocation(ShaderManager::GetShader("basicColor")->Get(), "Scale"));

	vb = std::make_unique<VertexBuffer>(&m_Vertices[0], sizeof(m_Vertices));
	va = std::make_unique<VertexArray>();
	layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(2);
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);

	m_Percentage = 0.f;
	m_Position = {};
	m_Scale = { 100.f, 100.f };
	m_Size = { 100.f, 100.f };

	EventManager::GetInstance()->OnRender.AddListener(std::bind(&ProgressBar::Draw, this));
}

ProgressBar::~ProgressBar()
{
	
}

void ProgressBar::SetPercentage(float a_Percentage)
{
	if(a_Percentage > 1.f)
	{
		m_Percentage = 1.f;
	}
	else {
		m_Percentage = a_Percentage;
	}
	m_Vertices[10] = m_Percentage;
	m_Vertices[15] = m_Percentage;
	m_Vertices[20] = m_Percentage;

	std::vector<float> vertices;
	for(int i = 0; i < 60; i++)
	{
		vertices.push_back(m_Vertices[i]);
	}
	vb->ChangeData(vertices);
}

float Trickster::ProgressBar::GetPercentage()
{
	return m_Percentage;
}

void ProgressBar::Draw()
{
	ShaderManager::GetShader("basicColor")->Bind();
	glm::vec2 ScreenPos = ToScreenPos();
	glUniform2f(m_UniformLoc, ScreenPos.x, ScreenPos.y);

	//glUniform2f(m_UniformLoc, 0.f, 0.f);
	glUniform2f(m_ScaleUniformLoc, 1, 1);

	vb->Bind();
	va->AddBuffer(*vb, *layout);
	va->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 12));
}
glm::vec2 ProgressBar::ToScreenPos()
{
	return { m_Position.x / (Application::Get()->GetWindow()->GetWidth() * 0.5f) - 1.f
		, m_Position.y / (Application::Get()->GetWindow()->GetHeight() * 0.5f) - 1.f };
}