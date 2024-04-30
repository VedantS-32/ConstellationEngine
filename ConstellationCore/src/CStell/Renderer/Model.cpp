#include "CStellpch.h"
#include "Model.h"

#include "Buffer.h"
#include "RenderCommand.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/transform.hpp>

namespace CStell
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 2.0f));
	static float delta = 0.0f;

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
			vertex.m_position = vector;

			vector.x = (float)mesh->mNormals[i].x;
			vector.y = (float)mesh->mNormals[i].y;
			vector.z = (float)mesh->mNormals[i].z;
			vertex.m_normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = (float)mesh->mTextureCoords[0][i].x;
				vec.y = (float)mesh->mTextureCoords[0][i].y;
				vertex.m_texcoords = vec;
			}
			else
				vertex.m_texcoords = glm::vec2(0.0f, 0.0f);

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

	static void processNode(Model& model, const aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model.GetMeshes().push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(model, node->mChildren[i], scene);
		}
	}

	static int loadModel(Model& model, const std::string filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CSTELL_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return -1;
		}
		model.SetFilepath(filepath.substr(0, filepath.find_last_of('/')));

		processNode(model, scene->mRootNode, scene);

		return 0;
	}

	Model::Model(const std::string& filepath)
	{
		loadModel(*this, filepath);

		m_VertexArray = VertexArray::Create();

		m_VertexArray->Bind();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (auto& mesh : m_Meshes)
		{
			for (auto& vertex : mesh.GetVertices())
				vertices.push_back(vertex);
			for (auto& index : mesh.GetIndices())
				indices.push_back(index);
		}
		
		m_VertexBuffer = VertexBuffer::Create((void*)vertices.data(), (uint32_t)vertices.size() * sizeof(Vertex));
		CSTELL_TRACE("Vertices size: {0}", (uint32_t)vertices.size() * sizeof(Vertex));
		m_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" }
			}
		);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		//m_IndexBuffer = IndexBuffer::Create(m_Meshes[0].GetIndices().data(), (uint32_t)m_Meshes[0].GetIndices().size());
		m_IndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
		CSTELL_TRACE("Indices count: {0}", indices.size());
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		
		m_Texture = Texture2D::Create("asset/texture/CStell.png");

		m_Material = CreateRef<Material>("asset/shader/3DTest.glsl");
		m_Material->AddTexture(m_Texture);
		m_Shader = m_Material->GetShader();
	}

	void Model::DrawModel(const EditorCamera& camera)
	{
		if (delta <= 0)
			delta += 0.1f;
		if (delta >= 1)
			delta -= 0.1f;
		glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, 2.0f);

		modelMat = glm::rotate(modelMat, glm::radians(delta), glm::vec3(1.0f, 1.0f, 1.0f));

		m_Texture->Bind();
		m_Shader->Bind();
		m_Shader->SetMat4f("u_MVP", camera.GetViewProjectionMatrix() * modelMat);
		m_Shader->SetMat4f("u_ModelView", camera.GetViewMatrix() * modelMat);
		m_Shader->Set3f("u_LightPosition", lightPos);
		m_Shader->Set3f("u_CameraPosition", camera.GetPosition());
		m_Shader->Set1i("u_Texture", 0);

		RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
		//m_Shader->Unbind();
		//m_VertexArray->Unbind();
	}
}
