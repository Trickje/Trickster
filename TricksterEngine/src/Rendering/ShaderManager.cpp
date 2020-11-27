#include "pch.h"
#include "ShaderManager.h"
#include "Shader.h"


namespace Trickster {
	ShaderManager* ShaderManager::instance = nullptr;



	Shader * ShaderManager::GetShader(const std::string a_FilePath)
	{
		auto it = ShaderManager::GetInstance()->map.find(a_FilePath);
		if (it != ShaderManager::GetInstance()->map.end())
		{
			//found
			return it->second;
		}
		return ShaderManager::GetInstance()->map[a_FilePath] = new Shader(a_FilePath);

	}





	ShaderManager * ShaderManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ShaderManager();
		}
		return instance;
	}

	void ShaderManager::Initialize()
	{
		GetShader("basic")->Bind();
		GetShader("basic")->SetUniform1i("ourTexture", 0);
		GetShader("3D");
	}

	ShaderManager::ShaderManager()
	{
	}


	ShaderManager::~ShaderManager()
	{
	}
}