#pragma once
#include "Rendering/Window.h"

namespace Trickster
{
	class RenderAPI
	{
	public:
		static RenderAPI* Create();
		
		TRICKSTER_API virtual ~RenderAPI(){};
		//A function to bind a shader
				//TRICKSTER_API virtual bool BindShader();
		//A function to get a uniform location
		/*
@Args
Shader shader - Shader to get location from
String name - Uniform name
		 */
				//TRICKSTER_API virtual int GetUniformLocation();
		//A function that draws vertices using the render api
/*
Draws the queue that the Render API has stored

 */


		TRICKSTER_API virtual void Initialize() = 0;
		TRICKSTER_API virtual void DrawFrame() = 0;
		TRICKSTER_API virtual void Resize(int width, int height) =0;
		TRICKSTER_API virtual void LoadModel(const std::string& a_ModelName) = 0;
		TRICKSTER_API virtual void DrawModel(const std::string& a_ModelName, const glm::mat4& a_ModelMatrix) = 0;
		TRICKSTER_API virtual void LoadSprite(const std::string& a_SpriteName) = 0;
		TRICKSTER_API virtual void DrawSprite(const std::string& a_SpriteName, const glm::mat4& a_ModelMatrix, const bool a_IsScreenSpace) = 0;
		/*
		TRICKSTER_API virtual bool Draw();
		TRICKSTER_API virtual void SetUniform2f();
		TRICKSTER_API virtual void SetUniform3f();
		TRICKSTER_API virtual void SetUniform4f();
		TRICKSTER_API virtual void SetUniformMatrix4f();
		TRICKSTER_API virtual void SetTexture();
		TRICKSTER_API virtual void BindVertexArray();
		TRICKSTER_API virtual void CreateViewport();
		TRICKSTER_API virtual void ClearScreen();
		TRICKSTER_API virtual void Destroy();
		*/
		//Stuff with buffers
		//Index buffers
		//Vertex buffers
		//Vertex description objects
		//Frame buffers?
		//Clear buffers
		//Color buffers

		//Render queue stuff
	protected:
		//std::shared_ptr<Window> m_Window;
	};
}
