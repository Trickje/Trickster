#pragma once

#include "Core/Logger.h"
#include "Core/Application.h"
#include "UI/ClickableBox.h"
#include "UI/ProgressBar.h"
#include "IronMan.h"
#include "Rendering/Camera.h"
#include "UI/UIClickable.h"
#include "Core/AudioPlayer.h"
namespace Trickster {
	class Game : public Trickster::Application {
	public:
		Game();
		~Game();
	
		void OnUpdate(float a_DeltaTime) override;
		void OnStart() override;
		void OnRender() override;
		void OnPause(bool isPaused) override;
		bool IsTickBased() override;
	private:
		IronMan* m_IronMan;
		std::shared_ptr<Camera> m_Camera;
		bool m_TickBased = true;
		ProgressBar* bar;
		UIClickable* m_PauseMenu;
		std::vector<IronMan*> m_Planets;
		int m_MainMusic;
		float volumex = -1.f;
	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
