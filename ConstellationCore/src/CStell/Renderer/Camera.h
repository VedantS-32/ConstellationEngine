#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace CStell
{
	class Camera
	{
	public:
		//Orthographic Camera
		Camera(double left, double right, double bottom, double top, double zNear, double zFar);

		// Perspective Camera
		Camera(float FOV, float aspect, double zNear, double zFar);

		void SetOrthographic(double left, double right, double bottom, double top, double zNear, double zFar);
		void SetPerspective(float FOV, float aspect, double zNear, double zFar);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_ViewProjectionMatrix{ 1.0f };

		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Orientation{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Up{ 0.0f, 0.0f, 1.0f };
	};
}