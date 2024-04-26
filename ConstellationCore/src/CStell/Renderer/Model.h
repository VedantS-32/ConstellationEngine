#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Texture.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "EditorCamera.h"

#include <vector>
#include "Shader.h"

namespace CStell
{
    struct Vertex {
        glm::vec3 m_position;
        glm::vec3 m_normal;
        glm::vec2 m_texcoords;
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

        Model(const std::string& filepath);

        //int loadModel(std::string filepath);

        virtual std::vector<Mesh>& GetMeshes() { return m_Meshes; }

        void SetFilepath(const std::string& filepath) { m_Filepath = filepath; }

        void DrawModel(const EditorCamera& camera);

    protected:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Texture2D> m_Texture;
        Ref<Shader> m_Shader;

        std::vector<Mesh> m_Meshes;
        std::string m_Filepath;
	};
}
