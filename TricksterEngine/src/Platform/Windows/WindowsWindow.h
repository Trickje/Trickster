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
		void SetKeyDown(int Key, bool a_bool);
		void SetFullscreen(bool a_Fullscreen) override;
		void SetClick(int MouseKey, bool value) override;
		void Resize(int a_Width, int a_Height) override;
		void* GetRaw() override;
		void CaptureMouse(bool yoinkMouse) override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		
		GLFWwindow* m_Window;
		
		std::string m_Title;
		unsigned int m_Width, m_Height;
		int m_PosX, m_PosY;
		bool m_vSync;

	};

}
