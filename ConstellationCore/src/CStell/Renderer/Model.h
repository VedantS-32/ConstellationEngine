#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Texture.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Material.h"
#include "EditorCamera.h"

#include <vector>

namespace CStell
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 Texcoords;
    };

    struct Mesh
    {
        CSTELL_API Mesh() = default;
        CSTELL_API Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
            : Vertices(vertices), Indices(indices)
        {
        }

        CSTELL_API  const std::vector<Vertex>& GetVertices() const { return Vertices; }
        CSTELL_API std::vector<uint32_t>& GetIndices() { return Indices; }
        CSTELL_API Ref<Material>& GetMaterial() { return m_Material; }

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        glm::mat4 m_ModelMatrix{ 1.0f };

        Ref<Material> m_Material;
        std::string m_MaterialPath;
    };

    class MeshAsset
    {
    public:
        friend class MeshSerializer;

        CSTELL_API MeshAsset();
        CSTELL_API MeshAsset(const std::string& filepath);
        CSTELL_API MeshAsset(const std::string& filepath, const std::string& materialFile);

        CSTELL_API static Ref<MeshAsset> Create(const std::string& filePath);

        CSTELL_API std::vector<Mesh>& GetMeshes() { return m_Meshes; }
        CSTELL_API void SetFilepath(const std::string& filepath) { m_Filepath = filepath; }
        CSTELL_API const std::string& GetFilepath() { return m_Filepath; }

        CSTELL_API void DrawModel(const EditorCamera& camera, int entityID);
        CSTELL_API void UpdateTransform(const glm::mat4& transform);

    private:
        void PrepareMesh(const std::string& filepath = "asset/model/CStellCube.obj", const std::string& materialPath = "asset/material/3DTest.csmat");

    protected:

        std::vector<Mesh> m_Meshes;
        std::string m_Filepath;
    };

    class Model
    {
    public:

        CSTELL_API Model();
        CSTELL_API Model(const std::string& filepath);

        CSTELL_API  Ref<MeshAsset> GetMeshAsset() { return m_MeshAsset; }

        CSTELL_API void SetFilepath(const std::string& Filepath) { m_Filepath = Filepath; }

        CSTELL_API void DrawModel(const EditorCamera& camera, int entityID);

        CSTELL_API void UpdateTransform(const glm::mat4& transform);

        std::string m_Filepath;

    private:
        Ref<MeshAsset> m_MeshAsset;
	};
}
