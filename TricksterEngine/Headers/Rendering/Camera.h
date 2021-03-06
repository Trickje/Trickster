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
	class Camera
	{
	public:
		TRICKSTER_API Camera();
		TRICKSTER_API ~Camera();
		TRICKSTER_API void LookAt(const glm::vec3& a_Target);

		//Sets the position in the view matrix
		TRICKSTER_API void SetPosition(const glm::vec3& a_Position);
		//Gets the position out of the view matrix
		TRICKSTER_API glm::vec3 GetPosition()const;
		//Sets the position in ScreenSpace
		TRICKSTER_API void SetScreenPos(const glm::vec2& a_Position);
		//Gets the position in ScreenSpace
		TRICKSTER_API const glm::vec2& GetScreenPos()const;
		//Sets LockRoll
		TRICKSTER_API void SetLockRoll(const bool a_Bool);
		//Gets LockRoll
		TRICKSTER_API bool GetLockRoll()const;
		//Moves the camera by an offset in the view matrix
		TRICKSTER_API void Move(const glm::vec3& a_Offset);
		//Moves the camera by an offset in ScreenSpace
		TRICKSTER_API void MoveScreen(const glm::vec2& a_Offset);
		//Sets the Field of View
		TRICKSTER_API void SetFOV(const float a_FOV);
		//Gets the Field of View
		TRICKSTER_API float GetFOV()const;
		//Sets the Aspect Ratio width/height
		TRICKSTER_API void SetAspectRatio(const float a_AspectRatio);
		//Gets the Aspect Ratio width/height
		TRICKSTER_API float GetAspectRatio()const;
		//Gets the view matrix stored within this object
		TRICKSTER_API const glm::mat4& GetView()const;
		//Gets the Up vector from the View matrix
		TRICKSTER_API glm::vec3 GetUp()const;
		//Gets the Right vector from the View matrix
		TRICKSTER_API glm::vec3 GetRight()const;
		//Gets the Forward vector from the View matrix
		TRICKSTER_API glm::vec3 GetForward()const;
		//Sets the size the camera displays on screen
		TRICKSTER_API void SetScreenSize(const glm::vec2& a_Size);
		//Gets the size the camera displays on screen
		TRICKSTER_API const glm::vec2& GetScreenSize()const;
		//Sets the near and far for the projection matrix
		TRICKSTER_API void SetNearFar(const float a_Near, const float a_Far);
		//Sets the near for the projection matrix
		TRICKSTER_API void SetNear(const float a_Near);
		//Sets the far for the projection matrix
		TRICKSTER_API void SetFar(const float a_Far);
		//Does the calculation of the View Projection matrix (Projection * View)
		TRICKSTER_API void RecalculateViewProjection();
		//Gets the View Projection. Multiply this by the Model matrix (Projection * View * Model)
		//Call this after you did the recalculation. (Which is done OnUpdate)
		TRICKSTER_API const glm::mat4& GetViewProjection()const;
		//Rotates around the axis of the camera (up forward and right).
		//Delta in degrees
		TRICKSTER_API void Rotate(const float a_DeltaYaw = 0.f, const float a_DeltaPitch = 0.f, const float a_DeltaRoll = 0.f);
		TRICKSTER_API glm::mat4 GetProjection();
		//Moves a normal camera around based on the x and y delta
		TRICKSTER_API void MouseMove(float a_X, float a_Y);
	private:
		//Only accessed within this class!
		//Sets the view
		TRICKSTER_API void SetView(const glm::mat4& a_View);
		TRICKSTER_API glm::mat4 CalculateProjection();

		glm::mat4 m_View{};
		glm::mat4 m_Projection{};
		glm::mat4 m_ViewProjection{};
		glm::vec3 m_Position{};
		float m_FOV;
		float m_AspectRatio;
		bool m_LockRoll;
		float m_Far;
		float m_Near;
		float m_PI = 3.14159265359f;
		float m_SensitivityX = 1.f;
		float m_SensitivityY = 1.f;
		glm::vec2 m_LastMousePos{};
		//To place it on the screen, maybe for multiple cameras
		glm::vec2 m_Size{};		//Width, Height
		glm::vec2 m_ScreenPos{};	//0,0 is bottom left
		glm::vec3 m_Forward{};
		glm::vec3 m_Right{};
		glm::vec3 m_Up{};
		float yaw, pitch, roll;
		bool firstMouse = true;

	};

	TRICKSTER_API inline void Camera::SetView(const glm::mat4& a_View)
	{
		m_View = a_View;
	}



}