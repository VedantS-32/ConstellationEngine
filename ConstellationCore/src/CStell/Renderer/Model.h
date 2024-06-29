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
    struct CSTELL_API Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 Texcoords;
    };

    struct CSTELL_API Mesh
    {
        Mesh() = default;
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
            : Vertices(vertices), Indices(indices)
        {
        }

        const std::vector<Vertex>& GetVertices() const { return Vertices; }
        std::vector<uint32_t>& GetIndices() { return Indices; }
        Ref<Material>& GetMaterial() { return m_Material; }

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        glm::mat4 m_ModelMatrix{ 1.0f };

        Ref<Material> m_Material;
        std::string m_MaterialPath;
<<<<<<< Updated upstream
    };

    class MeshAsset
    {
    public:
        friend class MeshSerializer;

        MeshAsset();
        MeshAsset(const std::string& filepath);
        MeshAsset(const std::string& filepath, const std::string& materialFile);

        static Ref<MeshAsset> Create(const std::string& filePath);

        bool Deserialize(const std::string& filepath);
        std::vector<Mesh>& GetMeshes() { return m_Meshes; }
        void SetMeshPath(const std::string& meshPath) { m_MeshPath = meshPath; }

        void DrawModel(const EditorCamera& camera, int entityID);
        void UpdateTransform(const glm::mat4& transform);

    private:
        void PrepareMesh(const std::string& filepath = "asset/model/CStellCube.obj", const std::string& materialPath = "asset/material/3DTest.csmat");

    protected:

        std::vector<Mesh> m_Meshes;
        std::string m_MeshPath;
=======
>>>>>>> Stashed changes
    };

    class CSTELL_API MeshAsset
    {
    public:
        friend class MeshSerializer;

        MeshAsset();
        MeshAsset(const std::string& filepath);
        MeshAsset(const std::string& filepath, const std::string& materialFile);

        static Ref<MeshAsset> Create(const std::string& filePath);

        bool Deserialize(const std::string& filepath);
        std::vector<Mesh>& GetMeshes() { return m_Meshes; }
        void SetMeshPath(const std::string& meshPath) { m_MeshPath = meshPath; }

        void DrawModel(const EditorCamera& camera, int entityID);
        void UpdateTransform(const glm::mat4& transform);

    private:
        void PrepareMesh(const std::string& filepath = "asset/model/CStellCube.obj", const std::string& materialPath = "asset/material/3DTest.csmat");

    protected:

        std::vector<Mesh> m_Meshes;
        std::string m_MeshPath;
    };

    class CSTELL_API Model
    {
    public:

        Model();
        Model(const std::string& filepath);

        Ref<MeshAsset> GetMeshAsset() { return m_MeshAsset; }

        void SetMeshPath(const std::string& meshPath) { m_MeshPath = meshPath; }

        void DrawModel(const EditorCamera& camera, int entityID);

        void UpdateTransform(const glm::mat4& transform);

        std::string m_MeshPath;

    private:
        Ref<MeshAsset> m_MeshAsset;
	};
}
