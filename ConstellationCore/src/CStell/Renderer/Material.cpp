#include "CStellpch.h"
#include "Material.h"

namespace CStell
{
	Material::Material()
	{
		m_Texture = Texture2D::Create("Resource/Icon/Cstell.png");
	}

	void Material::AddTexture(const Ref<Texture2D>& texture)
	{
		if (texture)
		{
			m_Textures.push_back(texture);
		}
	}
}
