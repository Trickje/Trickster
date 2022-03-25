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

namespace TE {
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};
	class Shader
	{
	public:
		Shader(const std::string& a_ShaderName);
		~Shader();
		void Bind() const;
		void Unbind() const;
		unsigned int Get();
		//Set uniforms
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniform1i(const std::string& name, int v0);

		int GetUniformLocation(const std::string& name);


	private:
		unsigned int m_RendererID;
		//Caching for uniforms
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::string m_FilePath;


		/*
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgramSource ParseShader(const std::string& filePath);
		*/
		unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	};

}