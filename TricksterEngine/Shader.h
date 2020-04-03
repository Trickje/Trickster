#pragma once
#include <string>
#include <unordered_map>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	unsigned int Get();
	//Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int v0);


	
	private:
	unsigned int m_RendererID;
	//Caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::string m_FilePath;

	
	int GetUniformLocation(const std::string& name);
	/*
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filePath);
	*/
	unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};

