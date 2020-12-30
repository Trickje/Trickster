#include "pch.h"
#include "Renderer.h"

#include "Texture.h"
#include "TextureManager.h"
#include "Drawable2D.h"
#include "MeshManager.h"
#include "SpriteManager.h"
#include "Window.h"
namespace Trickster {
	//#define QUADBASEDUI
#define TRIANGLEBASEDUI
	Renderer::Renderer()
	{
		LOG("[Renderer] Created Renderer");
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

#ifdef QUADBASEDUI
	void Renderer::DrawUI()
	{
		float vertices[] = {
			-0.5, 0.5, 0.0,
			0.5, 0.5, 0.0,
			0.5, -0.5, 0.0,
			-0.5, -0.5, 0.0
		};

		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		// This will identify our vertex buffer
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexBuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
		glDrawArrays(GL_QUADS, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
	}
#endif
}