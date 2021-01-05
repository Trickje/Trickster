#pragma once
namespace Trickster {
	class Shader;
	class ShaderManager
	{
	public:
		static Shader* GetShader(const std::string a_FilePath);
		static ShaderManager* GetInstance();
		void Initialize();
	private:
		static ShaderManager* instance;
		std::map<std::string, Shader*> map;
		ShaderManager();
		~ShaderManager();
	};
}