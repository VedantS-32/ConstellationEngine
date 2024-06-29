#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class Material;
	class CSTELL_API MaterialSerializer
	{
	public:
		MaterialSerializer(const Ref<Material> material);

		void Serialize(const std::string& filepath = "asset/material/3DTest.csmat");
		bool Deserialize(const std::string& filepath = "asset/material/3DTest.csmat");

	private:
		Ref<Material> m_Material;
	};
}