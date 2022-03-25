#pragma once
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Rendering/Camera.h"
#include "Core/AudioPlayer.h"

using namespace TE;
class Game : public Application {
public:
	Game();
	~Game();
	
	void OnUpdate(float a_DeltaTime) override;
	void OnStart() override;
	void OnRender() override;
	void OnPause(bool isPaused) override;
	bool IsTickBased() override;
private:
	std::shared_ptr<Camera> m_Camera;
	bool m_TickBased = true;
	int m_MainMusic;
	float volumex = 0.f;
	FMOD::ChannelGroup* m_MainGroup;
	std::vector<Drawable3D*> m_Objects;
	std::vector<Drawable2D*> m_Sprites;
};

//This Creates the application in the entry point
static Application* TE::CreateApplication() 
{
	return new Game();
}
