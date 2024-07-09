#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class Material;

	class MaterialSerializer
	{
	public:
		static void Serialize(Material* material);
		static bool Deserialize(Material* material);

		CSTELL_API static void Serialize(Ref<Material> material);
		CSTELL_API static bool Deserialize(Ref<Material> material);
	};
}