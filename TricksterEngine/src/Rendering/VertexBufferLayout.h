#pragma once
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