#pragma once
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Trickster {
	class WindowsWindow :
		public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.width; }
		inline unsigned int GetHeight() const override { return m_Data.height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.eventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void Draw() override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};
		WindowData m_Data;
	};

}
