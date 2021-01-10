#pragma once
/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
#include "Rendering/Window.h"

//#include <GLFW/glfw3.h>

namespace Trickster {
	
	class WindowsWindow :
		public Window
	{
	public:
		TRICKSTER_API WindowsWindow(const WindowProps& props);
		TRICKSTER_API virtual ~WindowsWindow();

		TRICKSTER_API void OnUpdate() override;

		TRICKSTER_API inline unsigned int GetWidth() const override { return m_Width; }
		TRICKSTER_API inline unsigned int GetHeight() const override { return m_Height; }
		
		//Window attributes
		TRICKSTER_API void EventHandle() override;
		TRICKSTER_API void SetVSync(bool enabled) override;
		TRICKSTER_API bool IsVSync() const override;
		TRICKSTER_API void Draw() override;
		TRICKSTER_API bool ShouldClose() const override;
		TRICKSTER_API void SetKeyDown(int Key, bool a_bool);
		TRICKSTER_API void SetFullscreen(bool a_Fullscreen) override;
		TRICKSTER_API void SetClick(int MouseKey, bool value) override;
		TRICKSTER_API void SetTitle(std::string a_Title) override;
		TRICKSTER_API void Resize(int a_Width, int a_Height) override;
		TRICKSTER_API void* GetRaw() override;
		TRICKSTER_API void CaptureMouse(bool yoinkMouse) override;
	private:
		TRICKSTER_API virtual void Init(const WindowProps& props);
		TRICKSTER_API virtual void Shutdown();
		
		GLFWwindow* m_Window;
		
		std::string m_Title;
		unsigned int m_Width, m_Height;
		int m_PosX, m_PosY;
		bool m_vSync;

	};

}
