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
        Mesh() = default;
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
            : Vertices(vertices), Indices(indices)
        {
        }

        const std::vector<Vertex>& GetVertices() const { return Vertices; }
        std::vector<uint32_t>& GetIndices() { return Indices; }

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

    };

    class Model
    {
    public:

        Model();
        Model(const std::string& filepath, const std::string& shaderFile);

        virtual std::vector<Mesh>& GetMeshes() { return m_Meshes; }
        Ref<Material> GetMaterial() { return m_Material; }

        void SetFilepath(const std::string& filepath) { m_Filepath = filepath; }

        void DrawModel(const EditorCamera& camera, int entityID);

        void UpdateTransform(const glm::mat4& transform);

    private:
        void PrepareMesh(const std::string& filepath = "asset/model/CStellCube.obj", const std::string& shaderPath = "asset/shader/3DTest.glsl");

    protected:
        glm::mat4 m_ModelMatrix{1.0f};

        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Texture2D> m_Texture;
        Ref<Material> m_Material;

        std::vector<Mesh> m_Meshes;
        std::string m_Filepath;
	};
}
