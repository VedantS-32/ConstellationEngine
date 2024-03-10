#pragma once

#include "Camera.h"
#include "Texture.h"

namespace CStell
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tiling);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tiling);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec4& tint, float tiling);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec4& tint, float tiling);
	};
}
