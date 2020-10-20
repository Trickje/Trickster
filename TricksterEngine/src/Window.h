#pragma once
#include "Engine.h"
#include "Events/Event.h"
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720
namespace Trickster {
	enum Keys
	{
		Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M, TAB, CAPSLOCK, SHIFT, CTRL, ALT, ENTER, LESS_THAN, GREATER_THAN, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO, MINUS, EQUALS, BACKSPACE, SLASH, SPACE, ESC, UP, RIGHT, LEFT, DOWN
	};
	enum Mouse
	{
		Left, Right, Middle, ScrollUp, ScrollDown, Four, Five
	};
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
		virtual bool GetKeyDown(int Key) = 0;
		virtual void SetFullscreen(bool a_Fullscreen) = 0;
		virtual void GetCursorPos(double* x, double* y) = 0;
		virtual bool GetClick(int MouseKey) = 0;
		virtual void SetClick(int MouseKey, bool value) = 0;
		
		static Window* Create(const WindowProps& props = WindowProps());
	protected:
		virtual void SetKeyDown(int Key, bool a_bool) = 0;
		
		
	};

}
