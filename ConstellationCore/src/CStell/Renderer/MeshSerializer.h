#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class MeshAsset;
	class MeshSerializer
	{
	public:
		MeshSerializer(Ref<MeshAsset> meshAsset);
		~MeshSerializer();

		void Serialize(const std::string& filepath = "asset/model/Sphere.csmesh");
		bool Deserialize(const std::string& filepath = "asset/model/Sphere.csmesh");

	private:
		Ref<MeshAsset> m_MeshAsset;
	};
}