#pragma once
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count, bool a_IsDynamic);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
	void ChangeData(std::vector<unsigned int> a_Data);
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};

