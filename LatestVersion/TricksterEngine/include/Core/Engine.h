#pragma once
namespace Trickster {
	class Renderer;
	class Engine
	{
	public:
		TRICKSTER_API Engine();
		TRICKSTER_API ~Engine();
		TRICKSTER_API bool Initialize();
		TRICKSTER_API void Update();
		Renderer* renderer;
		TRICKSTER_API static std::string GetResourcePath();
	};
}
