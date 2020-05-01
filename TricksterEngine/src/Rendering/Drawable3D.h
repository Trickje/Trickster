#pragma once
#include <glm/mat4x4.hpp>


#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


#define TINYOBJLOADER_IMPLEMENTATION
class Camera;

namespace Trickster {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 tex;
	};
	struct DrawData
	{
		std::shared_ptr<VertexBuffer> vb;
		std::shared_ptr<VertexBufferLayout> layout;
		std::shared_ptr<VertexArray> va;
	};
	class Drawable3D
	{
	public:
		Drawable3D();
		Drawable3D(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale = {1.f,1.f,1.f}, const std::string& a_ShaderFileName = "3D");
		~Drawable3D();
		//Does the same as the constructor but can be used from the child classes
		void Initialize(const std::string& a_ModelFileName, const glm::vec3& a_Position, const glm::vec3& a_Scale = { 1.f,1.f,1.f }, const std::string& a_ShaderFileName = "3D");
		//Sets the shader path
		void SetShaderPath(const std::string& a_FilePath);
		//Draws the model to a camera
		void Draw(Camera* a_Camera);
		//Loads the mesh with the given file path
		void LoadMesh(const std::string& a_FileName);
		//Sets the position in world space
		void SetPosition(const glm::vec3& a_Position);
		//Gets the position
		glm::vec3 GetPosition()const;
		//Moves the object in world space
		void Move(const glm::vec3& a_Offset);
		//Gets the forward vector from the model matrix
		glm::vec3 GetForward()const;
		//Gets the right vector from the model matrix
		glm::vec3 GetRight()const;
		//Gets the up vector from the model matrix
		glm::vec3 GetUp()const;
		//Sets the Model matrix so the object looks at the target position
		void LookAt(const glm::vec3& target, const glm::vec3& tmp = glm::vec3(0, 1, 0));
		//Sets the scale of the model
		void SetScale(const glm::vec3& a_Scale);

	private:
		std::string m_TextureBase;
		std::string m_TextureFile;
		std::string m_ShaderPath;
		void MakeBuffers();//Call this after you have data in m_Vertices


		/*
	Right |  Up |  Forward  |   Position
	-------------------------------
	|   x    |   x   |       x        |   x   |
	-------------------------------
	|   y    |   y   |       y        |   y   |
	-------------------------------
	|   z    |   z   |       z        |   z   |
	-------------------------------
Scale	|   x    |   y   |       z        |  1.f  |
	-------------------------------
		 */
		glm::mat4 m_ModelMatrix;
		std::shared_ptr<DrawData> m_DrawData;
		std::vector<std::shared_ptr<Vertex>> m_Vertices;	//Mesh
		
	};

}
