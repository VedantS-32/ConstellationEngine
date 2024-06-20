#include "CStellpch.h"
#include "MeshSerializer.h"

#include "Model.h"
#include "CStell/Core/AssetManager.h"

#include "CStell/Utils/YamlOperators.h"

namespace CStell
{
	MeshSerializer::MeshSerializer(Ref<MeshAsset> meshAsset)
		: m_MeshAsset(meshAsset)
	{
	}

	MeshSerializer::~MeshSerializer()
	{
		//delete m_MeshAsset;
	}

	void MeshSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "MeshAsset" << YAML::Value << filepath;
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginMap;

		int i = 0;
		for (auto& mesh : m_MeshAsset->GetMeshes())
		{
			out << YAML::Key << i << YAML::Value << mesh.m_Material->GetMaterialPath();
			i++;
		}

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool MeshSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["MeshAsset"])
			return false;

		std::string meshPath = data["MeshAsset"].as<std::string>();
		m_MeshAsset->PrepareMesh(meshPath);
		CSTELL_CORE_TRACE("Deserializing material '{0}'", meshPath);

		auto assetManager = AssetManager::GetInstance();
		auto texture = assetManager->LoadAsset<Texture2D>("asset/texture/CStell.png");

		auto material = data["Materials"];
		int i = 0;
		for (auto& mesh : m_MeshAsset->GetMeshes())
		{

			mesh.m_Material = assetManager->LoadAsset<Material>(material[i].as<std::string>());
			mesh.m_Material->AddTexture(texture);

			i++;
		}

		return true;
	}
}
