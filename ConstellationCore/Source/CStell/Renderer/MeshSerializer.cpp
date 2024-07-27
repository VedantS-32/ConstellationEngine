#include "CStellpch.h"
#include "MeshSerializer.h"

#include "Model.h"
#include "CStell/Core/AssetManager.h"

#include "CStell/Utils/YamlOperators.h"

namespace CStell
{
	void MeshSerializer::Serialize(Model* model)
	{
		auto meshAsset = model->GetMeshAsset();
		std::string filepath = model->GetFilepath();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "MeshAsset" << YAML::Value << meshAsset->GetFilepath();
		out << YAML::Key << "Name" << YAML::Value << meshAsset->GetMeshName();
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

		CSTELL_CORE_INFO("Serialized model: {0}", filepath);
	}

	bool MeshSerializer::Deserialize(Model* model)
	{
		const std::string filepath = model->GetFilepath();
		CSTELL_CORE_INFO("Deserializing model: {0}", filepath);

		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["MeshAsset"])
			return false;

		std::string meshPath = data["MeshAsset"].as<std::string>();

		auto assetManager = AssetManager::GetInstance();
		CSTELL_CORE_TRACE("Loading mesh: {0}", meshPath);
		model->m_MeshAsset = assetManager->LoadAsset<MeshAsset>(meshPath);

		auto meshAsset = model->GetMeshAsset();

		meshAsset->m_MeshName = data["Name"].as<std::string>();
		CSTELL_CORE_INFO("Loaded mesh: {0}", meshAsset->GetMeshName());

		//auto texture = assetManager->LoadAsset<Texture2D>("asset/texture/CStell.png");

		auto material = data["Materials"];
		int i = 0;
		for (auto& mesh : meshAsset->GetMeshes())
		{
			CSTELL_CORE_TRACE("Has material: {0}", material[i].IsDefined());
			if(material[i].IsDefined())
			{
				mesh.m_Material = assetManager->LoadAsset<Material>(material[i].as<std::string>());
				//mesh.m_Material->AddTexture(texture);
			}
			else
			{
				mesh.m_Material = assetManager->LoadAsset<Material>("asset/material/DefaultMat.csmat");
			}

			i++;
		}

		strStream.flush();
		stream.close();

		CSTELL_CORE_INFO("Deserialized model: {0}", filepath);
		return true;
	}

	void MeshSerializer::Serialize(Ref<Model> model)
	{
		Serialize(model.get());
	}

	bool MeshSerializer::Deserialize(Ref<Model> model)
	{
		return Deserialize(model.get());
	}
}
