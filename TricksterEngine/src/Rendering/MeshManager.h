#pragma once

class Camera;

namespace Trickster {
	class Drawable3D;
	class MeshManager
	{
	public:
		static MeshManager* GetInstance();
		std::vector<std::shared_ptr<Drawable3D>> m_Drawable3Ds;
		std::vector<std::string> m_TexturePaths;
		bool Initialize(std::shared_ptr<Camera> a_Camera);
		std::shared_ptr<Camera> GetCamera();
	private:
		static MeshManager* instance;
		std::shared_ptr<Camera> m_Camera;
		MeshManager();
		~MeshManager();
	};
}
