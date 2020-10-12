#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Events/EventManager.h"


Camera::Camera()
{
	Trickster::EventManager::GetInstance()->OnUpdate.AddListener(std::bind(&Camera::RecalculateViewProjection, this));
	m_View = glm::mat4(0.f);
	m_Projection = glm::mat4(0.f);
	m_ViewProjection = glm::mat4(0.f);
	m_Size = glm::vec2(100.f, 100.f);
	m_ScreenPos = glm::vec2(0.f);
	m_FOV = glm::radians(70.f);
	m_AspectRatio = 1.7777778f;
	m_LockRoll = true;
	m_Far = 100.f;
	m_Near = 0.1f;
}


Camera::~Camera()
{
	//Delete any raw pointers here
}

void Camera::LookAt(const glm::vec3& a_Target)
{
	m_View = glm::lookAt(m_Position, a_Target, glm::vec3{ 0.f, 1.f, 0.f });
	/*
	const glm::vec3 forward = glm::normalize(GetPosition() - a_Target);
	const glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), forward);
	const glm::vec3 up = glm::cross(forward, right);
	const glm::vec3 pos(GetPosition());
	*/
	/*
	glm::mat4 orientation = {
		glm::vec4(right.x, up.x, forward.x, 0),
	  glm::vec4(right.y, up.y, forward.y, 0),
	  glm::vec4(right.z, up.z, forward.z, 0),
	  glm::vec4(0,       0,       0,     1)
	};
	glm::mat4 translation = {
		glm::vec4(1,      0,      0,   0),
		glm::vec4(0,      1,      0,   0),
		glm::vec4(0,      0,      1,   0),
		glm::vec4(-pos.x, -pos.y, -pos.z, 1)
	};
	m_View = (orientation * translation);
	*/
	/*
	m_View = glm::mat4();
	m_View[0][0] = right.x;
	m_View[0][1] = right.y;
	m_View[0][2] = right.z;
	m_View[1][0] = up.x;
	m_View[1][1] = up.y;
	m_View[1][2] = up.z;
	m_View[2][0] = forward.x;
	m_View[2][1] = forward.y;
	m_View[2][2] = forward.z;

	m_View[3][0] = pos.x;
	m_View[3][1] = pos.y;
	m_View[3][2] = pos.z;
	*/
	/*
	const glm::vec3 forward = glm::normalize(GetPosition() - to);
	const glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), forward);
	const glm::vec3 up = glm::cross(forward, right);

	glm::mat4 camToWorld;

	camToWorld[0][0] = right.x;
	camToWorld[0][1] = right.y;
	camToWorld[0][2] = right.z;
	camToWorld[1][0] = up.x;
	camToWorld[1][1] = up.y;
	camToWorld[1][2] = up.z;
	camToWorld[2][0] = forward.x;
	camToWorld[2][1] = forward.y;
	camToWorld[2][2] = forward.z;

	camToWorld[3][0] = from.x;
	camToWorld[3][1] = from.y;
	camToWorld[3][2] = from.z;
	m_View = camToWorld;
	*/
}

void Camera::SetPosition(const glm::vec3& a_Position)
{
	m_Position = a_Position;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

void Camera::SetScreenPos(const glm::vec2& a_Position)
{
	m_ScreenPos = a_Position;
}

const glm::vec2 & Camera::GetScreenPos() const
{
	return m_ScreenPos;
}

void Camera::SetLockRoll(const bool a_Bool)
{
	m_LockRoll = a_Bool;
}

bool Camera::GetLockRoll() const
{
	return m_LockRoll;
}

void Camera::Move(const glm::vec3& a_Offset)
{
	m_View[3][0] += a_Offset.x;
	m_View[3][1] += a_Offset.y;
	m_View[3][2] += a_Offset.z;
}

void Camera::MoveScreen(const glm::vec2 & a_Offset)
{
	m_ScreenPos.x += a_Offset.x;
	m_ScreenPos.y += a_Offset.y;
}

void Camera::SetFOV(const float a_FOV)
{
	m_FOV = a_FOV;
}

float Camera::GetFOV() const
{
	return m_FOV;
}

void Camera::SetAspectRatio(const float a_AspectRatio)
{
	m_AspectRatio = a_AspectRatio;
}

float Camera::GetAspectRatio()const
{
	return m_AspectRatio;
}

const glm::mat4 & Camera::GetView() const
{
	return m_View;
}

glm::vec3 Camera::GetUp() const
{
	return glm::vec3(m_View[1][0], m_View[1][1], m_View[1][2]);
}

glm::vec3 Camera::GetRight() const
{
	return glm::vec3(m_View[0][0], m_View[0][1], m_View[0][2]);
}

glm::vec3 Camera::GetForward() const
{
	return glm::vec3(m_View[2][0], m_View[2][1], m_View[2][2]);
}

void Camera::SetScreenSize(const glm::vec2 & a_Size)
{
	m_Size = a_Size;
}

const glm::vec2& Camera::GetScreenSize() const
{
	return m_Size;
}

void Camera::SetNearFar(const float a_Near, const float a_Far)
{
	m_Near = a_Near;
	m_Far = a_Far;
}

void Camera::SetNear(const float a_Near)
{
	m_Near = a_Near;
}

void Camera::SetFar(const float a_Far)
{
	m_Far = a_Far;
}

void Camera::RecalculateViewProjection()
{
	m_ViewProjection = this->CalculateProjection() * m_View ;
}

const glm::mat4& Camera::GetViewProjection()const
{
	return m_ViewProjection;
}

void Camera::Rotate(const float a_DeltaYaw, const float a_DeltaPitch, const float a_DeltaRoll)
{
	m_View = glm::rotate(m_View, glm::radians(a_DeltaYaw), GetUp());
	m_View = glm::rotate(m_View, glm::radians(a_DeltaPitch), GetRight());
	m_View = glm::rotate(m_View, glm::radians(a_DeltaRoll), GetForward());
	
}
