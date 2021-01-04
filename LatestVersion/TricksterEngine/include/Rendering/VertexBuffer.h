#pragma once
namespace Trickster {
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(const void* data, unsigned int size, bool a_IsDynamic);
		~VertexBuffer();
		void ChangeData(std::vector<float>& a_Data);
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID;
	};

}