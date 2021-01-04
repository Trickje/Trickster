#pragma once
namespace Trickster {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		bool Initialize();
		void Draw();
		void DrawTransparent();
		void DrawUI();
	private:
		std::vector<std::future<void>> m_Futures;
	};
}
