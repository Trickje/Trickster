#pragma once

//#include "EntryPoint.h"
#include "Logger.h"
#include "Application.h"
#include "Nicolai.h"

namespace Trickster {
	class Game : public Trickster::Application {
	public:
		Game();
		~Game();
	
		void OnUpdate(float a_DeltaTime) override;
		void OnStart() override;
		void OnRender() override;
		void OnEventListen() override;
		void OnEventExecute() override;
	private:
		std::vector<Nicolai*> m_Nicolais;


	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
