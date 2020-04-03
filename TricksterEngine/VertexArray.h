#pragma once
namespace Trickster {
	class VertexBufferLayout;
	class VertexBuffer;

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID;
	};

}