#pragma once

#include "entt.hpp"

#include "CStell/Core/Timestep.h"
#include "CStell/Renderer/EditorCamera.h"

namespace CStell
{
	class Entity;
	class SceneHierarchyPanel;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(float width, float height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		float m_ViewportWidth = 1, m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}