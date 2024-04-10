#pragma once

#include "CStell/Renderer/Camera.h"
#include "CStell/Scene/SceneCamera.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>

namespace CStell
{
	struct TagComponent
	{
		std::string Tag;
		
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DeleteInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [this]() { Instance = new T(); };
			DeleteInstanceFunction = [this]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [this](ScriptableEntity* instance) { ((T*)Instance)->OnCreate(); };
			OnUpdateFunction = [this](ScriptableEntity* instance, Timestep ts) { ((T*)Instance)->OnUpdate(ts); };
			OnDestroyFunction = [this](ScriptableEntity* instance) { ((T*)Instance)->OnDestroy(); };
		}
	};
}