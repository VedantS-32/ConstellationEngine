#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class Material;
	class MaterialSerializer
	{
	public:
		CSTELL_API MaterialSerializer(const Ref<Material> material);

		CSTELL_API void Serialize(const std::string& filepath = "asset/material/3DTest.csmat");
		CSTELL_API bool Deserialize(const std::string& filepath = "asset/material/3DTest.csmat");

	private:
		Ref<Material> m_Material;
	};
}