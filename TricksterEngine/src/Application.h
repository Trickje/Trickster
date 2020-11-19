#pragma once
#define GLEW_STATIC
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <memory>

#include "Timer.h"
#include "Engine.h"
#include "Window.h"

namespace Trickster {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		static Application* Get();
		virtual void Start();
		virtual void OnUpdate(float a_DeltaTime) = 0;
		virtual void OnStart() = 0;
		virtual void OnRender() = 0;
		
		void Draw();
		bool Update();
		std::shared_ptr<Engine> GetEngine() const;
		std::shared_ptr<Window> GetWindow() const;
		virtual bool IsTickBased() = 0;
	private:
		static Application* m_Application;
		std::shared_ptr<Engine> m_Engine;
	protected:
		std::shared_ptr<Window> m_Window;
		Timer m_Timer;
		float TickTime = 0.6f;
		float CurrentTickTime;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
}
