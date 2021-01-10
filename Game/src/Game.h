#pragma once
/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
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
		float volumex = 0.f;
		FMOD::ChannelGroup* m_MainGroup;
	};

	//This Creates the application in the entry point
	static Trickster::Application* CreateApplication() 
	{
		return new Game();
	}
}
