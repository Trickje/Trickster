#pragma once
#include "Event.h"
namespace Trickster {
	struct InputEvents
	{

		Event<int> OnKeyPressed;
		Event<int> OnKeyReleased;
		Event<int, int> OnKeyRepeat;
		Event<int> OnMouseButtonPressed;
		Event<int> OnMouseButtonReleased;
		Event<float, float> OnMouseButtonScrolled;
		Event<float, float> OnMouseMoved;
	};
}