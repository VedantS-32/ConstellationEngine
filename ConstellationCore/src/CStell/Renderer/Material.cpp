#include "CStellpch.h"
#include "Material.h"

#include "glad/glad.h"

namespace CStell
{
	Material::Material(const std::string& shaderPath)
	{
		m_Shader = Shader::Create(shaderPath);
		m_Shader->ExtractShaderUniform(m_Uniforms);
	}

	void Material::AddTexture(const Ref<Texture2D>& texture)
	{
		if (texture)
		{
			m_Textures.push_back(texture);
		}
	}
}