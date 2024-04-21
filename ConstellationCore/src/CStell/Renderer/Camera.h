#pragma once

#include "CStell/Core/Timestep.h"
#include "CStell/Events/Event.h"
#include "CStell/Events/MouseEvent.h"

#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace CStell
{
	class Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		Camera() = default;
		Camera(float fov, double nearClip, double farClip);

		virtual ~Camera() = default;

		virtual void OnUpdate(Timestep ts);
		virtual void OnEvent(Event& e);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type);

		void SetOrthographic(float size, float nearClip = -1.0f, float farClip = 1.0f);
		void SetPerspective(float fov, double nearClip = 0.1f, double farClip = 10000.0f);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width, m_ViewportHeight = height; UpdateProjectionMatrix(); }

		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		void UpdateOrientation(const glm::vec2& delta);
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();
		void UpdateViewProjectionMatrix();

	protected:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = 45.0f;
		double m_PerspectiveNear = 0.01f, m_PerspectiveFar = 10000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1000.0f, m_OrthographicFar = 1000.0f;

		float m_AspectRatio = 1.778f;

		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_ViewProjectionMatrix{ 1.0f };

		glm::vec3 m_Position{ 0.0f, -10.0f, 5.0f };
		glm::vec3 m_Orientation{ 0.0f, 1.0f, -0.5f };
		glm::vec3 m_UpDirection{ 0.0f, 0.0f, 1.0f };

		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
	};
}