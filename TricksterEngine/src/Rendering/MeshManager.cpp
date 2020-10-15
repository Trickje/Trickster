#include "pch.h"
#include "MeshManager.h"
#include "Drawable3D.h"
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
	}


	MeshManager::~MeshManager()
	{
	}
	bool MeshManager::Initialize(std::shared_ptr<Camera> a_Camera){
		LOG("Initializing MeshManager.")
		m_Camera = a_Camera;
		return true;
	}
	std::shared_ptr<Camera> MeshManager::GetCamera()
	{
		return m_Camera;
	}
}