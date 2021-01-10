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