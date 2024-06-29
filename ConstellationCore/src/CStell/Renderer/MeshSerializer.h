#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class MeshAsset;
<<<<<<< Updated upstream
	class MeshSerializer
=======
	class CSTELL_API MeshSerializer
>>>>>>> Stashed changes
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