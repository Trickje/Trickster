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
namespace Trickster {
	enum Keys
	{
		Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M, TAB, CAPSLOCK, SHIFT, CTRL, ALT, ENTER, LESS_THAN, GREATER_THAN, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO, MINUS, EQUALS, BACKSPACE, SLASH, SPACE, ESC, UP, RIGHT, LEFT, DOWN
	};
	enum Mouse
	{
		Left, Right, Middle, ScrollUp, ScrollDown, Four, Five
	};
	class Input
	{
	public:
		
		TRICKSTER_API static bool GetKeyDown(int Key);
		TRICKSTER_API static void SetKeyDown(int Key, bool a_bool);
		TRICKSTER_API static bool GetClick(int MouseKey);
		TRICKSTER_API static void SetClick(int MouseKey, bool a_bool);
		TRICKSTER_API static glm::vec2 GetMousePos();
		TRICKSTER_API static void SetCursorInWindow(bool isInWindow);
	private:
		TRICKSTER_API bool GetKeyDownImpl(int Key);
		TRICKSTER_API void SetKeyDownImpl(int Key, bool a_bool);
		TRICKSTER_API bool GetClickImpl(int MouseKey);
		TRICKSTER_API void SetClickImpl(int MouseKey, bool a_bool);
		Input();
		~Input();
		static Input* instance;
		bool Keys[54];
		bool MouseKeys[8];
		bool CursorInWindow;
	};

}
