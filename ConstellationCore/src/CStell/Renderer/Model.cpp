#include "CStellpch.h"

#include "Model.h"

#include "Buffer.h"
#include "RenderCommand.h"

#include "CStell/Core/AssetManager.h"
#include "MeshSerializer.h"
#include "CStell/Utils/YamlOperators.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

namespace CStell
{
	static Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = (float)mesh->mVertices[i].x;
			vector.y = (float)mesh->mVertices[i].y;
			vector.z = (float)mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = (float)mesh->mNormals[i].x;
			vector.y = (float)mesh->mNormals[i].y;
			vector.z = (float)mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = (float)mesh->mTextureCoords[0][i].x;
				vec.y = (float)mesh->mTextureCoords[0][i].y;
				vertex.Texcoords = vec;
			}
			else
				vertex.Texcoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// process indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh(vertices, indices);
	}

	static void processNode(MeshAsset& meshAsset, const aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshAsset.GetMeshes().push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshAsset, node->mChildren[i], scene);
		}
	}

	static int loadMeshAsset(MeshAsset& meshAsset, const std::string filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CSTELL_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return -1;
		}
		meshAsset.SetMeshPath(filepath.substr(0, filepath.find_last_of('/')));

		processNode(meshAsset, scene->mRootNode, scene);

		return 0;
	}

	MeshAsset::MeshAsset()
	{
		MeshSerializer meshSerializer(Ref<MeshAsset>(this));
		CSTELL_CORE_ASSERT(false);
		meshSerializer.Deserialize("asset/model/Sphere.csmesh");
	}

	MeshAsset::MeshAsset(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	MeshAsset::MeshAsset(const std::string& filepath, const std::string& materialFile)
	{
		PrepareMesh(filepath, materialFile);
	}

	Ref<MeshAsset> MeshAsset::Create(const std::string& filePath)
	{
		return CreateRef<MeshAsset>(filePath);
	}

	bool MeshAsset::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["MeshAsset"])
			return false;

		std::string meshPath = data["MeshAsset"].as<std::string>();
		PrepareMesh(meshPath);
		CSTELL_CORE_TRACE("Deserializing material '{0}'", meshPath);

		auto assetManager = AssetManager::GetInstance();
		auto texture = assetManager->LoadAsset<Texture2D>("asset/texture/CStell.png");

		auto material = data["Materials"];
		int i = 0;
		for (auto& mesh : GetMeshes())
		{

			mesh.m_Material = assetManager->LoadAsset<Material>(material[i].as<std::string>());
			mesh.m_Material->AddTexture(texture);

			i++;
		}

		return true;
	}

	void MeshAsset::PrepareMesh(const std::string& filepath, const std::string& shaderPath)
	{
		loadMeshAsset(*this, filepath);

		for (auto& mesh : m_Meshes)
		{
			mesh.m_VertexArray = VertexArray::Create();
			mesh.m_VertexArray->Bind();

			mesh.m_VertexBuffer = VertexBuffer::Create((void*)mesh.Vertices.data(), (uint32_t)mesh.Vertices.size() * sizeof(Vertex));
			CSTELL_TRACE("Vertices size: {0}", (uint32_t)mesh.Vertices.size() * sizeof(Vertex));
			mesh.m_VertexBuffer->SetLayout(
				{
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float3, "a_Normal" },
					{ ShaderDataType::Float2, "a_TexCoord" }
				}
			);

			mesh.m_VertexArray->AddVertexBuffer(mesh.m_VertexBuffer);

			mesh.m_IndexBuffer = IndexBuffer::Create(mesh.Indices.data(), (uint32_t)mesh.Indices.size());
			CSTELL_TRACE("Indices count: {0}", mesh.Indices.size());
			mesh.m_VertexArray->SetIndexBuffer(mesh.m_IndexBuffer);
		}
	}

	void MeshAsset::DrawModel(const EditorCamera& camera, int entityID)
	{
		for (auto& mesh : m_Meshes)
		{
			auto& shader = mesh.m_Material->GetShader();

			mesh.m_Material->m_Textures[0]->Bind();
			shader->Bind();
			shader->SetMat4f("u_Model", mesh.m_ModelMatrix);
<<<<<<< Updated upstream
			shader->SetMat4f("u_ModelView", camera.GetViewMatrix() * mesh.m_ModelMatrix);			

			mesh.m_Material->m_Mat4Uniforms["u_ViewProjection"] = camera.GetViewProjectionMatrix();
			mesh.m_Material->m_Float3Uniforms["u_CameraPosition"] = camera.GetPosition();
			mesh.m_Material->UpdateShaderUniform("ModelCommons");
=======
			shader->SetMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());
			shader->SetMat4f("u_ModelView", camera.GetViewMatrix() * mesh.m_ModelMatrix);
			shader->Set3f("u_CameraPosition", camera.GetPosition());

			//mesh.m_Material->m_Mat4Uniforms["u_ViewProjection"] = camera.GetViewProjectionMatrix();
			//mesh.m_Material->m_Float3Uniforms["u_CameraPosition"] = camera.GetPosition();
			//mesh.m_Material->UpdateShaderUniform("ModelCommons");
>>>>>>> Stashed changes

			shader->Set1i("u_Texture", 0);
			shader->Set1i("u_EntityID", entityID);

			RenderCommand::DrawIndexed(mesh.m_VertexArray, mesh.m_VertexArray->GetIndexBuffer()->GetCount());
		}
	}

	void MeshAsset::UpdateTransform(const glm::mat4& transform)
	{
		for (auto& mesh : m_Meshes)
			mesh.m_ModelMatrix = transform;
	}

	Model::Model()
	{
<<<<<<< Updated upstream
		auto assetManager = AssetManager::GetInstance();
		m_MeshAsset = assetManager->LoadAsset<MeshAsset>("asset/model/Sphere.csmesh");
	}

	Model::Model(const std::string& filepath = "asset/model/Sphere.csmesh")
=======
		m_MeshPath = "asset/model/Sphere.csmesh";
		auto assetManager = AssetManager::GetInstance();
		m_MeshAsset = assetManager->LoadAsset<MeshAsset>(m_MeshPath);
	}

	Model::Model(const std::string& filepath)
		: m_MeshPath(filepath)
>>>>>>> Stashed changes
	{
		auto assetManager = AssetManager::GetInstance();
		m_MeshAsset = assetManager->LoadAsset<MeshAsset>(filepath);
	}

	void Model::DrawModel(const EditorCamera& camera, int entityID)
	{
		m_MeshAsset->DrawModel(camera, entityID);
	}

	void Model::UpdateTransform(const glm::mat4& transform)
	{
		m_MeshAsset->UpdateTransform(transform);
	}
}