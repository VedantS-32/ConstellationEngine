#pragma once

#include "CStell/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO : REMOVE THIS!
typedef unsigned int GLenum;

namespace CStell
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void Set1i(const std::string& name, int value) override;
		virtual void Set2i(const std::string& name, glm::uvec2 value) override;
		virtual void Set3i(const std::string& name, glm::uvec3 value) override;
		virtual void Set4i(const std::string& name, glm::uvec4 value) override;
		virtual void Set1iArray(const std::string& name, int* value, uint32_t count) override;

		virtual void Set1f(const std::string& name, float value) override;
		virtual void Set2f(const std::string& name, glm::vec2 value) override;
		virtual void Set3f(const std::string& name, glm::vec3 value) override;
		virtual void Set4f(const std::string& name, glm::vec4 value) override;

		virtual void SetMat3f(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetMat4f(const std::string& name, const glm::mat4& matrix) override;

		void RecompileShaders() override;

		// Set uniforms
		void UploadUniform1i(const std::string& name, int value);
		void UploadUniform2i(const std::string& name, glm::uvec2 value);
		void UploadUniform3i(const std::string& name, glm::uvec3 value);
		void UploadUniform4i(const std::string& name, glm::uvec4 value);
		void UploadUniform1iArray(const std::string& name, int* value, uint32_t count);

		void UploadUniform1f(const std::string& name, float value);
		void UploadUniform2f(const std::string& name, glm::vec2 value);
		void UploadUniform3f(const std::string& name, glm::vec3 value);
		void UploadUniform4f(const std::string& name, glm::vec4 value);

		void UploadUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4f(const std::string& name, const glm::mat4& matrix);

		uint32_t GetRendererID() const override { return m_RendererID; }

	private:
		void PrepareShader();
		std::string ParseShader(std::string filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
		std::string m_ShaderPath;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}