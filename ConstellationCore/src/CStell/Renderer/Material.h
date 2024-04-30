#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace CStell
{
	class Material
	{
	public:
		Material(const std::string& shaderPath);

	public:
		const Ref<Shader> GetShader() const { return m_Shader; }
		void AddTexture(const Ref<Texture2D>& texture);

	private:
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture2D>> m_Textures;
		std::unordered_map<std::string, uint32_t> m_Uniforms;
	};
}