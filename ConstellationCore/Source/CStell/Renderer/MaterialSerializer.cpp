#include "CStellpch.h"
#include "MaterialSerializer.h"
#include "CStell/Core/AssetManager.h"

#include "CStell/Renderer/Material.h"
#include "CStell/Utils/YamlOperators.h"

namespace CStell
{
    void MaterialSerializer::Serialize(Material* material)
    {
		std::string filepath = material->GetMaterialPath();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << material->GetShaderPath();
		out << YAML::Key << "Name" << YAML::Value << filepath.substr(filepath.find_last_of('/') + 1, filepath.find_last_of('.'));

		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
		for (const auto& texture : material->m_TexturesLut)
		{
			out << texture.second->GetFilepath();
		}
		out << YAML::EndSeq;

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

		CSTELL_CORE_INFO("Serialized material: {0}", filepath);
    }

	void MaterialSerializer::Serialize(Ref<Material> material)
	{
		return Serialize(material.get());
	}

    bool MaterialSerializer::Deserialize(Material* material)
    {
		std::string materialPath = material->GetMaterialPath();
		std::ifstream stream(materialPath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Material"])
			return false;

		material->m_MaterialName = materialPath.substr(materialPath.find_last_of("/") + 1, materialPath.find_last_of("."));
		material->m_ShaderPath = data["Material"].as<std::string>();

		CSTELL_CORE_TRACE("Shader path: {0}", material->m_ShaderPath);

		auto assetManager = AssetManager::GetInstance();

		auto textures = data["Textures"];
		CSTELL_CORE_TRACE("Number of Textures {0}", textures.size());
		for (int i = 0; i < textures.size(); i++)
		{
			//CSTELL_CORE_TRACE("Textures Path: {0}", textures[i]);
			std::string texturePath = textures[i].as<std::string>();
			std::string textureName = texturePath.substr(texturePath.find_last_of("/") + 1, texturePath.find_last_of("."));
			material->m_TexturesLut[textureName] = assetManager->LoadAsset<Texture2D>(texturePath);
		}

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

		strStream.flush();
		stream.close();

		CSTELL_CORE_TRACE("Deserialized material: {0}", materialPath);
        return true;
    }

	const std::string MaterialSerializer::DeserializeShaderPath(const std::string& materialPath)
	{
		std::ifstream stream(materialPath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Material"])
			return "Invalid Path";

		std::string shaderPath = data["Material"].as<std::string>();
		CSTELL_CORE_TRACE("Deserializing material: {0}", materialPath);

		strStream.flush();
		stream.close();

		return shaderPath;
	}

	bool MaterialSerializer::Deserialize(Ref<Material> material)
	{
		return Deserialize(material.get());
	}
}
