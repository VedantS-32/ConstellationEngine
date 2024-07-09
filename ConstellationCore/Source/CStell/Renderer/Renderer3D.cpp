#include "CStellpch.h"
#include "Renderer3D.h"

#include "VertexArray.h"

namespace CStell
{
	struct Renderer3DData
	{
		Ref<VertexArray> ModelVertexArray;
		Ref<VertexBuffer> ModelVertexBuffer;
		Ref<IndexBuffer> ModelIndexBuffer;
	};

	static Renderer3DData s_ModelData;

	void Renderer3D::Init()
	{
		s_ModelData.ModelVertexArray = VertexArray::Create();
	}

	void Renderer3D::Shutdown()
	{
	}

	void Renderer3D::BeginScene(EditorCamera& camera, Model& model)
	{
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::StartBatch()
	{
	}

	void Renderer3D::NextBatch()
	{
	}

	void Renderer3D::Flush()
	{
	}

	void Renderer3D::RenderModel(const Ref<Model>& model, const glm::mat4& transform, const Ref<Shader>& shader, const Ref<Texture2D>& texture)
	{
		
	}
}
