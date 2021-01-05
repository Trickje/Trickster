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