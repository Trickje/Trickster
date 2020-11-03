#pragma once

//#include "EntryPoint.h"
#include "Logger.h"
#include "Application.h"
#include "ClickableBox.h"
#include "IronMan.h"
#include "Nicolai.h"
#include "Rendering/Camera.h"
#include "Rendering/UIClickable.h"

namespace Trickster {
	class Game : public Trickster::Application {
	public:
		Game();
		~Game();
	
		void OnUpdate(float a_DeltaTime) override;
		void OnStart() override;
		void OnRender() override;
		bool IsTickBased() override;
	private:
		std::vector<Nicolai*> m_Nicolais;
		IronMan* m_IronMan;
		std::shared_ptr<Camera> m_Camera;
		UIClickable* box;
		bool m_TickBased = true;
	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
