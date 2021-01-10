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
#ifndef ENTRY
#define ENTRY
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Rendering/Window.h"
#ifdef _DEBUG
int main()
#else
int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
#endif
{
	
	Trickster::Application* app = Trickster::CreateApplication();
	app->Start();
	while(app->Update())
	{
		app->OnRender();
		app->Draw();
		app->GetWindow()->EventHandle();
	
	}
	delete app;
	return 0;
}
#endif
