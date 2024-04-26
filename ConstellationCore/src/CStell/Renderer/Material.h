#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace CStell
{
	class Material
	{
	public:
		Material();
		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_TilingFactor = 1.0f;
		float m_TexIndex = 0;

		Ref<Texture2D> m_Texture;
		std::vector<Ref<Texture2D>> m_Textures;
		Ref<Shader> m_Shader;

	public:
		void AddTexture(const Ref<Texture2D>& texture);
	};
}