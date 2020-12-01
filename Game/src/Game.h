#pragma once

//#include "EntryPoint.h"
#include "Core/Logger.h"
#include "Core/Application.h"
#include "UI/ClickableBox.h"
#include "UI/ProgressBar.h"
#include "IronMan.h"
#include "Nicolai.h"
#include "Rendering/Camera.h"
#include "UI/UIClickable.h"

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
		UIClickable* box2;
		bool m_TickBased = true;
		ProgressBar* bar;
	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
