#pragma once

#include <string>
#include <glm/glm.hpp>

namespace CStell
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ParseShader(std::string filename);
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}