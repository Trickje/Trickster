#pragma once
#include "Event.h"
namespace TE {
	struct WindowEvents
	{
		Event<> OnWindowClose;
		Event<int, int> OnWindowResize;
		Event<bool> OnWindowMinimize;
		Event<bool> OnWindowMaximize;
	};
}