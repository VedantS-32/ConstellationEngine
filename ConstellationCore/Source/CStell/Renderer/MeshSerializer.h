#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	class Model;

	class MeshSerializer
	{
	public:
		static void Serialize(Model* model);
		static bool Deserialize(Model* model);

		CSTELL_API static void Serialize(Ref<Model> model);
		CSTELL_API static bool Deserialize(Ref<Model> model);
	};
}