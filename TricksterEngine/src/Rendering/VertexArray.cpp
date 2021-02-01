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
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#ifdef TRICKSTER_OPENGL
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
#endif