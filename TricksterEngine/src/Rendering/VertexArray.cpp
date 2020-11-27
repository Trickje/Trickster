#include "pch.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Trickster {
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}


	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		this->Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];

			// 1st attribute buffer : vertices
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(
				(GLuint)i,                            // attribute 0. No particular reason for 0, but must match the layout in the shader.
				element.count,                // size
				GL_FLOAT,                     // type
				element.normalized,           // normalized?
				(GLsizei)layout.GetStride(),           // stride
				(const GLvoid*)(offset * sizeof(float))           // array buffer offset
			));
			offset += element.count;
			//	GLCall(glDisableVertexAttribArray(0));
		}
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}