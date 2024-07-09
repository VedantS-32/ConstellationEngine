#include "CStellpch.h"
#include "MeshSerializer.h"

#include "Model.h"
#include "CStell/Core/AssetManager.h"

#include "CStell/Utils/YamlOperators.h"

namespace CStell
{
	void MeshSerializer::Serialize(MeshAsset* meshAsset)
	{
		std::string filepath = meshAsset->GetFilepath();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "MeshAsset" << YAML::Value << filepath;
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginMap;

		int i = 0;
		for (auto& mesh : meshAsset->GetMeshes())
		{
			out << YAML::Key << i << YAML::Value << mesh.m_Material->GetMaterialPath();
			i++;
		}

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool MeshSerializer::Deserialize(MeshAsset* meshAsset)
	{
		std::string filepath = meshAsset->GetFilepath();

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["MeshAsset"])
			return false;

		std::string meshPath = data["MeshAsset"].as<std::string>();
		meshAsset->PrepareMesh(meshPath);
		CSTELL_CORE_TRACE("Deserializing material '{0}'", meshPath);

		auto assetManager = AssetManager::GetInstance();
		auto texture = assetManager->LoadAsset<Texture2D>("asset/texture/CStell.png");

		auto material = data["Materials"];
		int i = 0;
		for (auto& mesh : meshAsset->GetMeshes())
		{

			mesh.m_Material = assetManager->LoadAsset<Material>(material[i].as<std::string>());
			mesh.m_Material->AddTexture(texture);

			i++;
		}

		return true;
	}

	void MeshSerializer::Serialize(Ref<MeshAsset> meshAsset)
	{
		Serialize(meshAsset.get());
	}

	bool MeshSerializer::Deserialize(Ref<MeshAsset> meshAsset)
	{
		return Deserialize(meshAsset.get());
	}
}
