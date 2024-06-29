#include "CStellpch.h"
#include "Material.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

#include "CStell/Utils/YamlOperators.h"

#include <glad/glad.h>

namespace CStell
{
	Ref<Material> Material::Create(const std::string& materialPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLMaterial>(materialPath);
		}

		CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	bool Material::DeserializeAttributes(const std::string& materialPath)
	{
		std::ifstream stream(materialPath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Material"])
			return false;

		std::string shaderPath = data["Material"].as<std::string>();
		CSTELL_CORE_TRACE("Deserializing material '{0}'", shaderPath);

		auto attribute = data["Attributes"];
		for (auto& uniform : GetUniforms())
		{
			std::string uniformName = uniform.first;

			if (!attribute[uniformName].IsDefined())
			{
				CSTELL_CORE_WARN("No value for uniform: {0}", uniformName);
				continue;
			}

			switch (uniform.second)
			{
			case ShaderDataType::Int:
				AddUniformValue(uniformName, attribute[uniformName].as<int>());
				break;
			case ShaderDataType::Int2:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec2>());
				break;
			case ShaderDataType::Int3:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec3>());
				break;
			case ShaderDataType::Int4:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec4>());
				break;
			case ShaderDataType::Float:
				AddUniformValue(uniformName, attribute[uniformName].as<float>());
				break;
			case ShaderDataType::Float2:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::vec2>());
				break;
			case ShaderDataType::Float3:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::vec3>());
				break;
			case ShaderDataType::Float4:
				AddUniformValue(uniformName, attribute[uniformName].as<glm::vec4>());
				break;
			case ShaderDataType::Mat3:
				break;
			case ShaderDataType::Mat4:
				break;
			default:
				break;
			}
		}

		UpdateShaderUniform("ModelProps");

		return true;
	}

	void Material::AddTexture(const Ref<Texture2D>& texture)
	{
		if (texture)
		{
			m_Textures.push_back(texture);
		}
	}
}