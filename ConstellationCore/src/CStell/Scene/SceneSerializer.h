#pragma once

#include "CStell/Core/Core.h"
#include "Scene.h"

namespace CStell
{
	class SceneSerializer
	{
	public:
		CSTELL_API SceneSerializer(const Ref<Scene> scene);

		CSTELL_API void Serialize(const std::string& filepath);
		CSTELL_API void SerializeRuntime(const std::string& filepath);

		CSTELL_API bool Deserialize(const std::string& filepath);
		CSTELL_API bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}