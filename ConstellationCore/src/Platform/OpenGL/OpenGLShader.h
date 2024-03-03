#pragma once

#include "CStell/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace CStell
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value);

		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, glm::vec2 value);
		void SetUniform3f(const std::string& name, glm::vec3 value);
		void SetUniform4f(const std::string& name, glm::vec4 value);

		void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ParseShader(std::string filename);
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}