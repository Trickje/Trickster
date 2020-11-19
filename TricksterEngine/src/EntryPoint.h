#pragma once
#ifndef ENTRY
#define ENTRY
#include "Logger.h"
#include "Application.h"
#include "../../Game/src/Game.h"
int main()
{
	
	Trickster::Application* app = Trickster::CreateApplication();
	app->Start();
//	system("PAUSE");
//	Trickster::LOG_CLEAR();
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
