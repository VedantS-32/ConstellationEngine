#include "CStellpch.h"
#include "Material.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

#include "CStell/Utils/YamlOperators.h"

#include <glad/glad.h>

namespace CStell
{
	Ref<Material> Material::Create(const std::string& materialPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLMaterial>(materialPath);
		}

		CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	void Material::AddTexture(const Ref<Texture2D>& texture)
	{
		if (texture)
		{
			m_Textures.push_back(texture);
		}
	}
}