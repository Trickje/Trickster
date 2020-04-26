#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::LookAt(const glm::vec3& from, const glm::vec3& to, const glm::vec3& tmp)
{
	glm::vec3 forward = glm::normalize(from - to);
	glm::vec3 right = CrossProduct(glm::normalize(tmp), forward);
	glm::vec3 up = CrossProduct(forward, right);

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

	return camToWorld;
}
