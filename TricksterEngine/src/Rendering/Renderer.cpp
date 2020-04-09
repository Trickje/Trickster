#include "pch.h"
#include "Renderer.h"


#include <GLFW/glfw3.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "stb_image.h"
#include "Texture.h"
#include "TextureManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Drawable2D.h"
#include "Engine.h"
#include "SpriteManager.h"
namespace Trickster {
	//#define QUADBASEDUI
#define TRIANGLEBASEDUI
	Renderer::Renderer()
	{
	}

	Renderer::Renderer(GLFWwindow* a_window)
	{
		window = a_window;
	}


	Renderer::~Renderer()
	{
	}

	bool Renderer::Initialize()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		ShaderManager::GetShader("basic")->Bind();
		ShaderManager::GetShader("basic")->SetUniform1i("ourTexture", 0);

		TextureManager::GetTexture("image.png")->Bind();
		std::string str = "image.png";
		return true;
	}

	void Renderer::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//Render UI
		DrawUI();
		//Render Objects

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



#ifdef TRIANGLEBASEDUI
	void Renderer::DrawUI()
	{

		if (SpriteManager::GetInstance()->m_Drawable2Ds.empty())
		{
			LOG_ERROR("oopsie");
			return;
		}
		bool shouldDraw = false;
		std::vector<int> deletos;
		for (int i = 0; i < SpriteManager::GetInstance()->m_Drawable2Ds.size(); i++)
		{
			if (SpriteManager::GetInstance()->m_Drawable2Ds[i]->GetPosition().x - 0.5f * SpriteManager::GetInstance()->m_Drawable2Ds[i]->GetSize().x > WINDOWWIDTH || SpriteManager::GetInstance()->m_Drawable2Ds[i]->GetPosition().y - 0.5F * SpriteManager::GetInstance()->m_Drawable2Ds[i]->GetSize().y > WINDOWHEIGHT)
			{
				deletos.push_back(i);
			}
			else
			{
				shouldDraw = true;
				SpriteManager::GetInstance()->m_Drawable2Ds[i]->Draw();
			}
		}
		for (int i = 0; i < deletos.size(); i++)
		{
			LOG_ERROR("HMMM");
			SpriteManager::GetInstance()->m_Drawable2Ds.erase(SpriteManager::GetInstance()->m_Drawable2Ds.begin() + deletos[i] - i);
		}

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