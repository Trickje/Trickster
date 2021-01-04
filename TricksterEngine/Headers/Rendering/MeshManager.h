#pragma once


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
