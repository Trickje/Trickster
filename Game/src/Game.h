#pragma once

//#include "EntryPoint.h"
#include "Logger.h"
#include "Application.h"
#include "IronMan.h"
#include "Nicolai.h"
#include "Rendering/Camera.h"

namespace Trickster {
	class Game : public Trickster::Application {
	public:
		Game();
		~Game();
	
		void OnUpdate(float a_DeltaTime) override;
		void OnStart() override;
		void OnRender() override;
	private:
		std::vector<Nicolai*> m_Nicolais;
		IronMan* m_IronMan;
		Camera* m_Camera;

	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
