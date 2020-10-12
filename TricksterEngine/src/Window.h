#pragma once
#include "Engine.h"
#include "Events/Event.h"
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720
namespace Trickster {
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(
			const std::string& a_Title = "Trickster Engine", 
			unsigned int a_Width = WINDOWWIDTH,
			unsigned int a_Height = WINDOWHEIGHT)
			: title(a_Title), width(a_Width), height(a_Height)
		{
		}
	};

	class Window
	{
	public:

		virtual ~Window(){}

		virtual void OnUpdate() = 0;
		virtual void Draw() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool ShouldClose() const = 0;
		//Window attributes
		virtual void EventHandle() = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());


		
	};

}
