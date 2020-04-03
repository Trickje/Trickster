#pragma once
#include <glm/vec2.hpp>

#include "Drawable.h"
class VertexBufferLayout;
class VertexArray;
class VertexBuffer;
class Drawable;
namespace Trickster {
	struct DrawData2D
	{
		DrawData2D(void* data, int size)
		{
			vb = new VertexBuffer(data, size);
			va = new VertexArray();
			layout = new VertexBufferLayout();
		};
		VertexBuffer* vb;
		VertexArray* va;
		VertexBufferLayout* layout;
	};

	class Drawable2D :
		protected Drawable
	{
	public:
		Drawable2D();
		Drawable2D(const glm::vec2 a_Position, const glm::vec2 a_Size, const std::string& a_FilePath);
		virtual ~Drawable2D();
		virtual void Draw() override;
		void SetSize(const float a_Width, const float a_Height);
		void SetTexture(const std::string& a_FilePath);
		glm::vec2 GetSize() const;
		void SetPosition(const float a_X, const float a_Y);
		glm::vec2 GetPosition() const;

		//TODO Delete this function
		std::vector<float> GetVertices();

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		//Texture file path
		std::string m_FilePath;
		DrawData2D* m_DrawData;

		//Location of the ScreenPos uniform in the shader
		GLint m_UniformLoc;
	private:
		float m_Vertices[12] =
		{
			0.f, 0.f,	//Bottom left
			0.f, 1.f,	//Top left
			1.f, 1.f,	//Top right
			1.f, 1.f,	//Top right
			1.f, 0.f,	//Bottom right
			0.f, 0.f	//Bottom left
		};

	};


}