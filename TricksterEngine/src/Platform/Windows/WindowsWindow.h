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

		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }

		//Window attributes
		void EventHandle() override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void Draw() override;
		bool ShouldClose() const override;
		bool GetKeyDown(int Key) override;
		void SetKeyDown(int Key, bool a_bool) override;
		void SetFullscreen(bool a_Fullscreen) override;
		void GetCursorPos(double* x, double* y) override;
		bool GetClick(int MouseKey) override;
		void SetClick(int MouseKey, bool value) override;
		void Resize(float a_Width, float a_Height) override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		
		GLFWwindow* m_Window;
		
		bool Keys[54];
		bool MouseKeys[8];
		std::string m_Title;
		unsigned int m_Width, m_Height;
		int m_PosX, m_PosY;
		bool m_vSync;

	};

}
