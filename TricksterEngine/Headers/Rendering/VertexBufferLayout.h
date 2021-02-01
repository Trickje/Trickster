#pragma once
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
#ifdef TRICKSTER_OPENGL
namespace Trickster {
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
	};
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		~VertexBufferLayout();
		template<typename T>
		void Push(unsigned int count, bool a_Normal = false)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count, bool a_Normal)
		{
			m_Elements.push_back({ GL_FLOAT,count, a_Normal });
			m_Stride += 4 * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count, bool a_Normal)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, a_Normal });
			m_Stride += 4 * count;
		}
		
		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	};

}
#endif