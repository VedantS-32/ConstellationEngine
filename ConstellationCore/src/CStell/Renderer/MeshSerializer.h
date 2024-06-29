#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class MeshAsset;
	class MeshSerializer;

	class MeshSerializer
	{
	public:
		CSTELL_API MeshSerializer(Ref<MeshAsset> meshAsset);
		CSTELL_API ~MeshSerializer();

		CSTELL_API void Serialize(const std::string& filepath = "asset/model/Sphere.csmesh");
		CSTELL_API bool Deserialize(const std::string& filepath = "asset/model/Sphere.csmesh");

	private:
		Ref<MeshAsset> m_MeshAsset;
	};
}