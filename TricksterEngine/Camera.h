#pragma once
#include <glm/mat4x4.hpp>


class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& tmp = glm::vec3(0, 1, 0));
private:
	inline glm::vec3 CrossProduct(const glm::vec3& v_A, const glm::vec3& v_B);
	glm::vec3 m_Right;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::mat4 m_Eye;
};

inline glm::vec3 Camera::CrossProduct(const glm::vec3& v_A, const glm::vec3& v_B)
{
	float c_P1 = v_A[1] * v_B[2] - v_A[2] * v_B[1];
	float c_P2 = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
	float c_P3 = v_A[0] * v_B[1] - v_A[1] * v_B[0];
	return { c_P1, c_P2, c_P3 };
}
