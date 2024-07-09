#pragma once

#include "EditorCamera.h"
#include "Model.h"
#include "Shader.h"

namespace CStell
{
	class CSTELL_API Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(EditorCamera& camera, Model& model);
		static void EndScene();

		static void StartBatch();
		static void NextBatch();
		static void Flush();

		static void RenderModel(const Ref<Model>& model, const glm::mat4& transform, const Ref<Shader>& shader, const Ref<Texture2D>& texture);
	};
}
