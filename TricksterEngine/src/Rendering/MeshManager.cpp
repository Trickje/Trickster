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
#include "pch.h"
#include "Rendering/MeshManager.h"
#include "Rendering/Drawable3D.h"
#include "Rendering/Camera.h"
namespace Trickster {
	MeshManager* MeshManager::instance = nullptr;
	MeshManager * MeshManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new MeshManager();
		}
		return instance;
	}

	MeshManager::MeshManager()
	{
#ifdef DETAILED_CONSOLE
		LOG("[MeshManager] Created MeshManager");
#endif
	}


	MeshManager::~MeshManager()
	{
	}
	bool MeshManager::Initialize(std::shared_ptr<Camera> a_Camera){
		m_Camera = a_Camera;
		return true;
	}
	std::shared_ptr<Camera> MeshManager::GetCamera()
	{
		return m_Camera;
	}
}