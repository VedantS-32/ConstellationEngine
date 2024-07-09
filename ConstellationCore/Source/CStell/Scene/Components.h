#pragma once

#include "CStell/Renderer/Camera.h"
#include "CStell/Scene/SceneCamera.h"
#include "CStell/Renderer/Material.h"
#include "CStell/Renderer/Model.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace CStell
{
	struct TagComponent
	{
		std::string Tag;
		
		CSTELL_API TagComponent() = default;
		CSTELL_API TagComponent(const TagComponent&) = default;
		CSTELL_API TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		CSTELL_API TransformComponent() = default;
		CSTELL_API TransformComponent(const TransformComponent&) = default;
		CSTELL_API TransformComponent(const glm::vec3 translation)
			: Translation(translation) {}

		CSTELL_API glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		CSTELL_API SpriteRendererComponent() = default;
		CSTELL_API SpriteRendererComponent(const SpriteRendererComponent&) = default;
		CSTELL_API SpriteRendererComponent(const glm::vec4& color)
		{
			Color = color;
		}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct ModelComponent
	{
		Model ModelInst;

		CSTELL_API ModelComponent() = default;
		CSTELL_API ModelComponent(const ModelComponent&) = default;
		CSTELL_API ModelComponent(const std::string& filepath)
		{
			ModelInst = Model(filepath);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CSTELL_API CameraComponent() = default;
		CSTELL_API CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		CSTELL_API void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}