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
#include "pch.h"
#include "Rendering/Renderer.h"


#include "Core/Application.h"
#include "Rendering/Texture.h"
#include "Rendering/TextureManager.h"
#include "Rendering/Drawable2D.h"
#include "Rendering/MeshManager.h"
#include "Rendering/SpriteManager.h"
#include "Rendering/Window.h"
#include "Rendering/Drawable3D.h"
#include "Events/EventManager.h"
#include "Rendering/Text/TextRenderer.h"

namespace Trickster {
#define TRIANGLEBASEDUI
	Renderer::Renderer()
	{
#ifdef DETAILED_CONSOLE
		LOG("[Renderer] Created Renderer");
#endif
	}

	//Renderer::Renderer(GLFWwindow* a_window)
//	{
		//window = a_window;
//	}


	Renderer::~Renderer()
	{
	}

	bool Renderer::Initialize()
	{
		
		EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Renderer::Draw, this));
		EventManager::GetInstance()->GameLoopEvents.OnRender.AddListener(std::bind(&Renderer::DrawTransparent, this));
		return true;
	}

	void Renderer::Draw()
	{
		std::vector<std::shared_ptr<Drawable3D>> drawables = MeshManager::GetInstance()->m_Drawable3Ds;

		if(drawables.empty())
		{
			LOG_WARNING("Empty mesh list")
		}
		for(int i = 0; i < drawables.size(); i++)
		{
			drawables[i]->Draw(MeshManager::GetInstance()->GetCamera());
		}
		DrawUI();
	}

	void Renderer::DrawTransparent()
	{
		/*
		auto drawables = SpriteManager::GetInstance()->m_Drawable2Ds;
		for(auto& drawable : drawables)
		{
			if(drawable->m_Transparent)
			{
				drawable->Draw();
			}
		}*/
	}


#ifdef TRIANGLEBASEDUI
	void Renderer::DrawUI()
	{
		if (Application::Get()->Paused()) return;
		if (SpriteManager::GetInstance()->m_Drawable2Ds.empty())
		{
			//LOG_WARNING("Empty UI list");
			return;
		}
		bool shouldDraw = false;
		std::vector<int> deletos;
		for (int i = 0; i < SpriteManager::GetInstance()->m_Drawable2Ds.size(); i++)
		{
				shouldDraw = true;
				if (!SpriteManager::GetInstance()->m_Drawable2Ds[i]->m_Transparent) {
					SpriteManager::GetInstance()->m_Drawable2Ds[i]->Draw();
				}
			
		}
		/*
		for (int i = 0; i < deletos.size(); i++)
		{
		//	LOG_ERROR("HMMM");
		//	SpriteManager::GetInstance()->m_Drawable2Ds.erase(SpriteManager::GetInstance()->m_Drawable2Ds.begin() + deletos[i]);
		}
		*/
	}
#endif

}
