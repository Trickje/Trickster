#pragma once
#include <glm/mat4x4.hpp>


class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& tmp = glm::vec3(0, 1, 0));
	
	//Sets the position in the view matrix
	void SetPosition(const glm::vec3& a_Position);
	//Gets the position out of the view matrix
	glm::vec3 GetPosition()const;
	//Sets the position in ScreenSpace
	void SetScreenPos(const glm::vec2& a_Position);
	//Gets the position in ScreenSpace
	const glm::vec2& GetScreenPos()const;
	//Sets LockRoll
	void SetLockRoll(const bool a_Bool);
	//Gets LockRoll
	bool GetLockRoll()const;
	//Moves the camera by an offset in the view matrix
	void Move(const glm::vec3& a_Offset);
	//Moves the camera by an offset in ScreenSpace
	void MoveScreen(const glm::vec2& a_Offset);
	//Sets the Field of View
	void SetFOV(const float a_FOV);
	//Gets the Field of View
	float GetFOV()const;
	//Sets the Aspect Ratio width/height
	void SetAspectRatio(const float a_AspectRatio);
	//Gets the Aspect Ratio width/height
	float GetAspectRatio()const;
	//Gets the view matrix stored within this object
	const glm::mat4& GetView()const;
	//Gets the Up vector from the View matrix
	glm::vec3 GetUp()const;
	//Gets the Right vector from the View matrix
	glm::vec3 GetRight()const;
	//Gets the Forward vector from the View matrix
	glm::vec3 GetForward()const;
	//Sets the size the camera displays on screen
	void SetScreenSize(const glm::vec2& a_Size);
	//Gets the size the camera displays on screen
	const glm::vec2& GetScreenSize()const;
	//Sets the near and far for the projection matrix
	void SetNearFar(const float a_Near, const float a_Far);
	//Sets the near for the projection matrix
	void SetNear(const float a_Near);
	//Sets the far for the projection matrix
	void SetFar(const float a_Far);
	//Does the calculation of the View Projection matrix (Projection * View)
	void RecalculateViewProjection();
	//Gets the View Projection. Multiply this by the Model matrix (Projection * View * Model)
	//Call this after you did the recalculation. (Which is done OnUpdate)
	const glm::mat4& GetViewProjection()const;
private:
	//Only accessed within this class!
	//Sets the view
	void SetView(const glm::mat4& a_View);
	const glm::mat4& CalculateProjection();
	
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_ViewProjection;
	float m_FOV;
	float m_AspectRatio;
	bool m_LockRoll;
	float m_Far;
	float m_Near;
	float m_PI = 3.14159265359f;
	//To place it on the screen, maybe for multiple cameras
	glm::vec2 m_Size;		//Width, Height
	glm::vec2 m_ScreenPos;	//0,0 is bottom left
	
};

inline void Camera::SetView(const glm::mat4 & a_View)
{
	m_View = a_View;
}

inline const glm::mat4 & Camera::CalculateProjection()
{
	const auto Scale = 1 / tan((m_FOV * 0.0174532925f) * 0.5f * m_PI / 180);
	m_Projection = glm::mat4(0.f);
	m_Projection[0][0] = Scale; // scale the x coordinates of the projected point 
	m_Projection[1][1] = Scale; // scale the y coordinates of the projected point 
	m_Projection[2][2] = -m_Far / (m_Far - m_Near); // used to remap z to [0,1] 
	m_Projection[2][3] = -m_Far * m_Near / (m_Far - m_Near); // used to remap z [0,1] 
	m_Projection[3][2] = -1; // set w = -z 
	m_Projection[3][3] = 0;
	return m_Projection;
}

