#pragma once
#define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#define WINDOWWIDTH 1024
#define WINDOWHEIGHT 768
#include "Renderer.h"

class Engine
{
public:
	Engine();
	~Engine();
	bool Initialize();
	void Update();
private:
	GLFWwindow* window;
	Renderer* renderer;
};
