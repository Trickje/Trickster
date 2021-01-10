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
	class Camera;
	class Drawable3D;
	class MeshManager
	{
	public:
		TRICKSTER_API static MeshManager* GetInstance();
		std::vector<std::shared_ptr<Drawable3D>> m_Drawable3Ds;
		std::vector<std::string> m_TexturePaths;
		TRICKSTER_API bool Initialize(std::shared_ptr<Camera> a_Camera);
		TRICKSTER_API std::shared_ptr<Camera> GetCamera();
	private:
		static MeshManager* instance;
		std::shared_ptr<Camera> m_Camera;
		TRICKSTER_API MeshManager();
		TRICKSTER_API ~MeshManager();
	};
}
