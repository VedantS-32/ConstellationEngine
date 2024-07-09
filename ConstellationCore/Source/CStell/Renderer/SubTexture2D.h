#pragma once


#include <glm/glm.hpp>

#include "Texture.h"

namespace CStell
{
	class SubTexture2D
	{
	public:
		CSTELL_API SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		CSTELL_API const Ref<Texture2D> GetTexture() const { return m_Texture; }
		CSTELL_API const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		CSTELL_API static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.0f, 1.0f });

	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}