#include "pch.h"
#include "Rendering/RenderAPI/OpenGL.h"

void Trickster::OpenGL::Initialize()
{
#ifdef TRICKSTER_OPENGL
	LOG("Rendering API: OpenGL");
#ifdef DETAILED_CONSOLE
	LOG("[Window] Initializing GLEW.");
#endif
	glewExperimental = GL_TRUE;
	glewInit();

#ifdef __DEBUG
	glDebugMessageCallback(DebugMessageCallback, 0);
#endif
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
#endif
}
