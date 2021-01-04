#pragma once
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
