#include "CStellpch.h"
#include "MaterialSerializer.h"

#include "CStell/Renderer/Material.h"
#include "CStell/Utils/YamlOperators.h"

namespace CStell
{
    void MaterialSerializer::Serialize(Material* material)
    {
		std::string filepath = material->GetMaterialPath();
		YAML::Emitter out;
		out << YAML::BeginMap;
		std::string shaderPath = filepath.substr(0, filepath.find_last_of('.')) + ".glsl";
		out << YAML::Key << "Material" << YAML::Value << shaderPath;
		out << YAML::Key << "Attributes" << YAML::Value << YAML::BeginMap;

		for (auto& uniform : material->GetUniforms())
		{
			std::string uniformName = uniform.first;
			switch (uniform.second)
			{
			case ShaderDataType::Int:
				out << YAML::Key << uniformName << YAML::Value << material->m_IntUniforms[uniformName];
				break;
			case ShaderDataType::Int2:
				out << YAML::Key << uniformName << YAML::Value << material->m_Int2Uniforms[uniformName];
				break;
			case ShaderDataType::Int3:
				out << YAML::Key << uniformName << YAML::Value << material->m_Int3Uniforms[uniformName];
				break;
			case ShaderDataType::Int4:
				out << YAML::Key << uniformName << YAML::Value << material->m_Int4Uniforms[uniformName];
				break;
			case ShaderDataType::Float:
				out << YAML::Key << uniformName << YAML::Value << material->m_FloatUniforms[uniformName];
				break;
			case ShaderDataType::Float2:
				out << YAML::Key << uniformName << YAML::Value << material->m_Float2Uniforms[uniformName];
				break;
			case ShaderDataType::Float3:
				out << YAML::Key << uniformName << YAML::Value << material->m_Float3Uniforms[uniformName];
				break;
			case ShaderDataType::Float4:
				out << YAML::Key << uniformName << YAML::Value << material->m_Float4Uniforms[uniformName];
				break;
			case ShaderDataType::Mat3:
				break;
			case ShaderDataType::Mat4:
				break;
			default:
				break;
			}
		}

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
    }

	void MaterialSerializer::Serialize(Ref<Material> material)
	{
		return Serialize(material.get());
	}

    bool MaterialSerializer::Deserialize(Material* material)
    {
		std::ifstream stream(material->GetMaterialPath());
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Material"])
			return false;

		std::string shaderPath = data["Material"].as<std::string>();
		CSTELL_CORE_TRACE("Deserializing material '{0}'", shaderPath);

		auto attribute = data["Attributes"];
		for (auto& uniform : material->GetUniforms())
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
				material->AddUniformValue(uniformName, attribute[uniformName].as<int>());
				break;
			case ShaderDataType::Int2:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec2>());
				break;
			case ShaderDataType::Int3:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec3>());
				break;
			case ShaderDataType::Int4:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::uvec4>());
				break;
			case ShaderDataType::Float:
				material->AddUniformValue(uniformName, attribute[uniformName].as<float>());
				break;
			case ShaderDataType::Float2:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::vec2>());
				break;
			case ShaderDataType::Float3:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::vec3>());
				break;
			case ShaderDataType::Float4:
				material->AddUniformValue(uniformName, attribute[uniformName].as<glm::vec4>());
				break;
			case ShaderDataType::Mat3:
				break;
			case ShaderDataType::Mat4:
				break;
			default:
				break;
			}
		}

		material->UpdateShaderUniform("ModelProps");

        return true;
    }
	bool MaterialSerializer::Deserialize(Ref<Material> material)
	{
		return Deserialize(material.get());
	}
}
