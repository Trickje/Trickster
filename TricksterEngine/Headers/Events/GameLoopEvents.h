#pragma once
#include "Event.h"
namespace TE {
	struct GameLoopEvents
	{
		Event<> OnRender;
		Event<> OnRenderTransparent;
		Event<float> OnUpdate;
		Event<> OnStart;
		Event<> Tick;
		Event<> TickOnce;
		Event<> PreTick;
		Event<> PostTick;
	};
}