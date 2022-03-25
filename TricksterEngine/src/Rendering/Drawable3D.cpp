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
#include "pch.h"
#include "Rendering/Drawable3D.h"
#include "Core/Application.h"
#include "Core/FilePaths.h"
#include "Events/EventManager.h"
#include "Rendering/RenderAPI/RenderAPI.h"

namespace TE {
	//Don't use this constructor!!
	Drawable3D::Drawable3D()
		: m_ModelMatrix(glm::mat4(1.0)), m_RotationMatrix(glm::mat4(1.0f)), m_TranslationMatrix(glm::mat4(1.0f))
	{
		m_ShaderPath = "";
	}
	
	Drawable3D::Drawable3D(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale,
		const std::string& a_ShaderFileName)
		: m_ModelMatrix(glm::mat4(1.0)), m_RotationMatrix(glm::mat4(1.0f)), m_TranslationMatrix(glm::mat4(1.0f)), m_ScaleMatrix(glm::mat4(1.0f))
	{
		Initialize(a_ModelFileName, a_Position, a_Scale, a_ShaderFileName);
		
	}


	Drawable3D::~Drawable3D()
	{
	}

	void Drawable3D::Initialize(const std::string & a_ModelFileName, const glm::vec3 & a_Position, const glm::vec3 & a_Scale, const std::string & a_ShaderFileName)
	{
		//Initializes data
		m_ModelMatrix = glm::mat4(1.0f);
		m_RotationMatrix = glm::mat4(1.0f);
		m_TranslationMatrix = glm::translate(glm::identity<glm::mat4>(), a_Position);
		m_ScaleMatrix = glm::scale(glm::identity<glm::mat4>(), a_Scale);
		m_Position = a_Position;
		m_Yaw = 0.f;
		m_Pitch = 0.f;
		m_Roll = 0.f;
		//Fills in data
		m_ModelName = a_ModelFileName;
		EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Drawable3D::Draw, this));
	}

	void Drawable3D::SetShaderPath(const std::string & a_FilePath)
	{
		m_ShaderPath = a_FilePath;
	}


	//This is handled by the engine. Don't manually draw it
	void Drawable3D::Draw()
	{
		Application::Get()->m_RenderAPI->DrawModel(m_ModelName, m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix);
	}



	void Drawable3D::SetPosition(const glm::vec3& a_Position)
	{
		m_Position = a_Position;
	}

	glm::vec3 Drawable3D::GetPosition() const
	{
		return m_Position;
	}

	void Drawable3D::Move(const glm::vec3& a_Offset)
	{
		m_Position += a_Offset;
	}

	glm::vec3 Drawable3D::GetForward() const
	{
		return glm::vec3(m_ModelMatrix[2][0],m_ModelMatrix[2][1], m_ModelMatrix[2][2]);
	}

	glm::vec3 Drawable3D::GetRight() const
	{
		return glm::vec3(m_ModelMatrix[0][0], m_ModelMatrix[0][1], m_ModelMatrix[0][2]);
	}

	glm::vec3 Drawable3D::GetUp() const
	{
		return glm::vec3(m_ModelMatrix[1][0], m_ModelMatrix[1][1], m_ModelMatrix[1][2]);
	}

	void Drawable3D::LookAt(const glm::vec3 & target, const glm::vec3 & tmp)
	{
		const glm::vec3 forward = glm::normalize(GetPosition() - target);
		const glm::vec3 right = glm::cross(glm::normalize(tmp), forward);
		const glm::vec3 up = glm::cross(forward, right);
		const glm::vec3 pos(GetPosition());
		m_ModelMatrix = glm::mat4();
		m_ModelMatrix[0][0] = right.x;
		m_ModelMatrix[0][1] = right.y;
		m_ModelMatrix[0][2] = right.z;
		m_ModelMatrix[1][0] = up.x;
		m_ModelMatrix[1][1] = up.y;
		m_ModelMatrix[1][2] = up.z;
		m_ModelMatrix[2][0] = forward.x;
		m_ModelMatrix[2][1] = forward.y;
		m_ModelMatrix[2][2] = forward.z;

		m_ModelMatrix[3][0] = pos.x;
		m_ModelMatrix[3][1] = pos.y;
		m_ModelMatrix[3][2] = pos.z;
	}

	void Drawable3D::SetScale(const glm::vec3 & a_Scale)
	{
		m_ModelMatrix[0][3] = a_Scale.x;
		m_ModelMatrix[1][3] = a_Scale.y;
		m_ModelMatrix[2][3] = a_Scale.z;
	}

	void Drawable3D::Rotate(const float a_DeltaYaw, const float a_DeltaPitch, const float a_DeltaRoll)
	{
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaYaw), GetUp());
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaPitch), GetRight());
		//m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(a_DeltaRoll), GetForward());
		
		m_Yaw += a_DeltaYaw;
		m_Pitch += a_DeltaPitch;
		m_Roll += a_DeltaRoll;
		
	}

	void Drawable3D::CalculateRotationMatrix()
	{
		m_RotationMatrix = glm::identity<glm::mat4>();
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Yaw), glm::vec3(0.f,1.f,0.f));
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Pitch), glm::vec3(1.f, 0.f, 0.f));
		m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_Roll), glm::vec3(0.f, 0.f, 1.f));
	}

	void Drawable3D::CalculateTranslationMatrix()
	{
		m_TranslationMatrix[3][0] = m_Position.x;
		m_TranslationMatrix[3][1] = m_Position.y;
		m_TranslationMatrix[3][2] = m_Position.z;
	}

}
