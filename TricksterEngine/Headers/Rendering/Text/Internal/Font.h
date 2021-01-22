#pragma once
#include "Structs.h"

/*
================================================================================
		Copyright 2021 Rick Pijpers

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
	class Font
	{
	public:
		Font();
		~Font();
		TRICKSTER_API void LoadFromFile(const std::string& a_FileName);
		TRICKSTER_API void Render();
		TRICKSTER_API bool operator==(const Font& other);
		TRICKSTER_API void AddVertexData(std::string text, float x, float y, float scale, glm::vec4 color);
	private:
		TRICKSTER_API void Initialize();
		std::string m_FileName;
		std::map<char, Character> m_Characters;
		unsigned int m_VAO, m_VBO;
		float atlas_width;
		float atlas_height;
		GLuint tex;
		struct point {
			float x;
			float y;
			float z;
			float tx;
			float ty;
			float r;
			float g;
			float b;
			float a;
		};
		std::vector<point> coords;
	};
}