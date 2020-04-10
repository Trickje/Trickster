#pragma once
#define GLEW_STATIC
#include <future>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
namespace Trickster {
	class Window;
	class Shader;
	class VertexBuffer;
	class VertexArray;
	class VertexBufferLayout;
	class IndexBuffer;
	class Drawable2D;

	class Renderer
	{
	public:
		Renderer();
	//	Renderer(GLFWwindow* a_window);
		~Renderer();
		bool Initialize();
		void Draw();
		void DrawUI();
	private:
		//TODO get this window out of this class
	//	GLFWwindow* window;
		std::vector<std::future<void>> m_Futures;
	};
}