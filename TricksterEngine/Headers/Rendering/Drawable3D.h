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
	
	class Drawable3D
	{
	public:
		TRICKSTER_API Drawable3D();
		TRICKSTER_API Drawable3D(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale = {1.f,1.f,1.f}, const std::string& a_ShaderFileName = "3D");
		TRICKSTER_API ~Drawable3D();
		//Does the same as the constructor but can be used from the child classes
		TRICKSTER_API void Initialize(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale = { 1.f,1.f,1.f }, const std::string& a_ShaderFileName = "3D");
		//Sets the shader path
		TRICKSTER_API void SetShaderPath(const std::string& a_FilePath);
		//Draws the model to a camera
		TRICKSTER_API void Draw();
		//Loads the mesh with the given file path
		//TRICKSTER_API void LoadMesh(const std::string& a_FileName);
		//Sets the position in world space
		TRICKSTER_API void SetPosition(const glm::vec3& a_Position);
		//Gets the position
		TRICKSTER_API glm::vec3 GetPosition()const;
		//Moves the object in world space
		TRICKSTER_API void Move(const glm::vec3& a_Offset);
		//Gets the forward vector from the model matrix
		TRICKSTER_API glm::vec3 GetForward()const;
		//Gets the right vector from the model matrix
		TRICKSTER_API glm::vec3 GetRight()const;
		//Gets the up vector from the model matrix
		TRICKSTER_API glm::vec3 GetUp()const;
		//Sets the Model matrix so the object looks at the target position
		TRICKSTER_API void LookAt(const glm::vec3& target, const glm::vec3& tmp = glm::vec3(0, 1, 0));
		//Sets the scale of the model
		TRICKSTER_API void SetScale(const glm::vec3& a_Scale);
		//Rotates around the axis of the object (up forward and right).
		//Delta in degrees
		TRICKSTER_API void Rotate(const float a_DeltaYaw = 0.f, const float a_DeltaPitch = 0.f, const float a_DeltaRoll = 0.f);

	private:
		TRICKSTER_API void CalculateRotationMatrix();
		TRICKSTER_API void CalculateTranslationMatrix();
		std::string m_TextureBase;
		std::string m_TextureFile;
		std::string m_ShaderPath;
		std::string m_ModelName;


		glm::mat4 m_ModelMatrix;
		glm::mat4 m_RotationMatrix;
		glm::mat4 m_ScaleMatrix;
		float m_Yaw, m_Pitch, m_Roll;
		glm::vec3 m_Position;
		glm::mat4 m_TranslationMatrix;
		
	};

}
