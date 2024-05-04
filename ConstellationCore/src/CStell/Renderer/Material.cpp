#include "CStellpch.h"
#include "Material.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLMaterial.h"

#include <glad/glad.h>

namespace CStell
{
	Ref<Material> Material::Create(const std::string& shaderPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLMaterial>(shaderPath);
		}

		CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	void Material::AddTexture(const Ref<Texture2D>& texture)
	{
		if (texture)
		{
			m_Textures.push_back(texture);
		}
	}

	template<typename T>
	T& Material::GetUniformValue(std::string uniformName, T value) { return T(); }

	template<>
	int& Material::GetUniformValue<int>(std::string uniformName, int value) { return m_IntUniforms[uniformName]; }
	template<>
	glm::uvec2& Material::GetUniformValue<glm::uvec2>(std::string uniformName, glm::uvec2 value) { return m_Int2Uniforms[uniformName]; }
	template<>
	glm::uvec3& Material::GetUniformValue<glm::uvec3>(std::string uniformName, glm::uvec3 value) { return m_Int3Uniforms[uniformName]; }
	template<>
	glm::uvec4& Material::GetUniformValue<glm::uvec4>(std::string uniformName, glm::uvec4 value) { return m_Int4Uniforms[uniformName]; }

	template<>
	float& Material::GetUniformValue<float>(std::string uniformName, float value) { return m_FloatUniforms[uniformName]; }
	template<>
	glm::vec2& Material::GetUniformValue<glm::vec2>(std::string uniformName, glm::vec2 value) { return m_Float2Uniforms[uniformName]; }
	template<>
	glm::vec3& Material::GetUniformValue<glm::vec3>(std::string uniformName, glm::vec3 value) { return m_Float3Uniforms[uniformName]; }
	template<>
	glm::vec4& Material::GetUniformValue<glm::vec4>(std::string uniformName, glm::vec4 value) { return m_Float4Uniforms[uniformName]; }

	//template<>
	//glm::mat3 Material::GetUniformValue<glm::mat3>(std::string uniformName, glm::mat4 value) { return m_Mat4Uniforms[uniformName]; }
	//template<>
	//glm::mat4 Material::GetUniformValue<glm::mat4>(std::string uniformName, glm::mat3 value) { return m_Mat3Uniforms[uniformName]; }
}