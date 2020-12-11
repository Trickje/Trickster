#pragma once
namespace Trickster {
	class VertexArray;
	class VertexBuffer;
	class VertexBufferLayout;
	class Camera;
	struct Vertex
	{
			float x, y, z, nx, ny, nz, u, v;
	};
	struct DrawData 
	{
		VertexBuffer* vb;
		VertexBufferLayout* layout;
		VertexArray* va;
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
		void Draw(std::shared_ptr<Camera> a_Camera);
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
		//Rotates around the axis of the object (up forward and right).
		//Delta in degrees
		void Rotate(const float a_DeltaYaw = 0.f, const float a_DeltaPitch = 0.f, const float a_DeltaRoll = 0.f);

	private:
		void CalculateRotationMatrix();
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
		glm::mat4 m_RotationMatrix;
		float m_Yaw, m_Pitch, m_Roll;
		glm::mat4 m_TranslationMatrix;
		std::shared_ptr<DrawData> m_DrawData;
		std::vector<Vertex> m_Vertices;	//Mesh
		bool m_IsLoaded;
		bool HasBuffers = false;
		
	};

}
