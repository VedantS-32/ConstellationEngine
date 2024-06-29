#pragma once

#include "entt.hpp"

#include "CStell/Core/Timestep.h"
#include "CStell/Renderer/EditorCamera.h"

namespace CStell
{
	class Entity;

	class Scene
	{
	public:
		CSTELL_API Scene();
		CSTELL_API ~Scene();

		CSTELL_API Entity CreateEntity(const std::string& name = std::string());
		CSTELL_API void DestroyEntity(Entity entity);

		CSTELL_API void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		CSTELL_API void OnUpdateRuntime(Timestep ts);
		CSTELL_API void OnViewportResize(float width, float height);

		CSTELL_API const entt::registry& GetRegistry() { return m_Registry; }
		CSTELL_API Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		float m_ViewportWidth = 1, m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneSerializer;
	};
}