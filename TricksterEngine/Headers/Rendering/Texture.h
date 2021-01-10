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
	class Texture
	{
	public:
		Texture(const std::string& a_FileName);
		~Texture();
		void Bind(unsigned int slot = 0) const;
		static void Unbind();
		int GetWidth() const;
		int GetHeight() const;
		int GetBPP() const;
		glm::vec2 GetScale() const;
	private:
		unsigned int m_RendererID;
		std::string m_FileName;
		unsigned char* m_LocalBuffer;
		int m_Width;
		int m_Height;
		int m_BPP;
	};

}