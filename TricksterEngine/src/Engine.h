#pragma once
#include "Rendering/Renderer.h"

namespace Trickster {
	class Engine
	{
	public:
		Engine();
		~Engine();
		bool Initialize();
		void Update();
		Renderer* renderer;
		static std::string GetResourcePath();
	};
}
