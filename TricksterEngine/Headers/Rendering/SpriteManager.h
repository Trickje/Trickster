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
	class Drawable2D;
	class SpriteManager
	{
	public:
		static SpriteManager* GetInstance();
		std::vector<Drawable2D*> m_Drawable2Ds;
		std::vector<std::string> m_TexturePaths;
	private:
		static SpriteManager* instance;
		SpriteManager();
		~SpriteManager();
	};
}