#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class MeshAsset;

	class MeshSerializer
	{
	public:
		static void Serialize(MeshAsset* meshAsset);
		static bool Deserialize(MeshAsset* meshAsset);

		CSTELL_API static void Serialize(Ref<MeshAsset> meshAsset);
		CSTELL_API static bool Deserialize(Ref<MeshAsset> meshAsset);
	};
}