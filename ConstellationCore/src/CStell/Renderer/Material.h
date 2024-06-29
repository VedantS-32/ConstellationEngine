#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace CStell
{
	class CSTELL_API Material
	{
	public:
		virtual ~Material() = default;

	public:
		static Ref<Material> Create(const std::string& shaderPath);

		const std::string& GetMaterialPath() const { return m_MaterialPath; }

		virtual void RecompileShaders() = 0;
		const Ref<Shader>& GetShader() const { return m_Shader; }
		virtual void ExtractShaderUniform(std::string uniformBlockName) = 0;
		virtual void UpdateShaderUniform(std::string UBOName) = 0;

		const std::unordered_map<std::string, ShaderDataType>& GetUniforms() const { return m_Uniforms; }

		void AddTexture(const Ref<Texture2D>& texture);

		void AddUniformValue(std::string uniformName, int value) { m_IntUniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::uvec2 value) { m_Int2Uniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::uvec3 value) { m_Int3Uniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::uvec4 value) { m_Int4Uniforms[uniformName] = value; }

		void AddUniformValue(std::string uniformName, float value) { m_FloatUniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::vec2 value) { m_Float2Uniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::vec3 value) { m_Float3Uniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::vec4 value) { m_Float4Uniforms[uniformName] = value; }

		void AddUniformValue(std::string uniformName, glm::mat4 value) { m_Mat4Uniforms[uniformName] = value; }
		void AddUniformValue(std::string uniformName, glm::mat3 value) { m_Mat3Uniforms[uniformName] = value; }

	protected:
		bool DeserializeAttributes(const std::string& materialPath);

	public:
		std::string m_MaterialPath;

		Ref<Shader> m_Shader;

		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;

		Ref<UniformBuffer> m_UniformBuffer;
		Ref<UniformBuffer> m_UniformBufferModel;
		std::vector<Ref<Texture2D>> m_Textures;
		std::unordered_map<std::string, ShaderDataType> m_Uniforms;

		std::unordered_map<std::string, int> m_IntUniforms;
		std::unordered_map<std::string, glm::ivec2> m_Int2Uniforms;
		std::unordered_map<std::string, glm::ivec3> m_Int3Uniforms;
		std::unordered_map<std::string, glm::ivec4> m_Int4Uniforms;

		std::unordered_map<std::string, float> m_FloatUniforms;
		std::unordered_map<std::string, glm::vec2> m_Float2Uniforms;
		std::unordered_map<std::string, glm::vec3> m_Float3Uniforms;
		std::unordered_map<std::string, glm::vec4> m_Float4Uniforms;

		std::unordered_map<std::string, glm::mat3> m_Mat3Uniforms;
		std::unordered_map<std::string, glm::mat4> m_Mat4Uniforms;
	};
}