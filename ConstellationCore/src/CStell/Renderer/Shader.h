#pragma once

#include "ShaderType.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace CStell
{
	class Shader
	{
	public:
		CSTELL_API virtual ~Shader() = default;

		CSTELL_API virtual void Bind() const = 0;
		CSTELL_API virtual void Unbind() const = 0;

		CSTELL_API virtual void RecompileShaders() = 0;

		CSTELL_API virtual void Set1i(const std::string& name, int value) = 0;
		CSTELL_API virtual void Set2i(const std::string& name, glm::uvec2 value) = 0;
		CSTELL_API virtual void Set3i(const std::string& name, glm::uvec3 value) = 0;
		CSTELL_API virtual void Set4i(const std::string& name, glm::uvec4 value) = 0;
		CSTELL_API virtual void Set1iArray(const std::string& name, int* value, uint32_t count) = 0;

		CSTELL_API virtual void Set1f(const std::string& name, float value) = 0;
		CSTELL_API virtual void Set2f(const std::string& name, glm::vec2 value) = 0;
		CSTELL_API virtual void Set3f(const std::string& name, glm::vec3 value) = 0;
		CSTELL_API virtual void Set4f(const std::string& name, glm::vec4 value) = 0;

		CSTELL_API virtual void SetMat3f(const std::string& name, const glm::mat3& matrix) = 0;
		CSTELL_API virtual void SetMat4f(const std::string& name, const glm::mat4& matrix) = 0;

		CSTELL_API virtual const std::string& GetName() const = 0;
		CSTELL_API virtual uint32_t GetUniformBlockIndex(const std::string& blockName) = 0;

		CSTELL_API virtual uint32_t GetRendererID() const = 0;

		CSTELL_API static Ref<Shader> Create(const std::string& filepath);
		CSTELL_API static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}