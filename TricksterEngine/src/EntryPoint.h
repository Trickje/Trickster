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
	system("PAUSE");
	Trickster::LOG_CLEAR();
	while(app->Update())
	{
		app->OnEventListen();
		app->OnEventExecute();
		app->OnRender();
		if(glfwGetKey(app->GetEngine()->window,GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(app->GetEngine()->window, 1);
		}
	}
	delete app;
	return 0;
}

#endif
