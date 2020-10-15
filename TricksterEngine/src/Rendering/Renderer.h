#pragma once
#define GLEW_STATIC
#include <future>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Drawable3D.h"

namespace Trickster {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		bool Initialize();
		void Draw();
		void DrawUI();
	private:
		std::vector<std::future<void>> m_Futures;
	};
}
