#pragma once
namespace Trickster {
	class Renderer;
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
