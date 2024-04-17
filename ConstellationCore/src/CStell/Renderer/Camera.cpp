#include "CStellpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace CStell
{
	Camera::Camera(float fov, double nearClip, double farClip)
		: m_PerspectiveFOV(fov), m_PerspectiveNear(nearClip), m_PerspectiveFar(farClip)
	{
		m_ProjectionType = Camera::ProjectionType::Perspective;
		UpdateProjectionMatrix();
	}

	void Camera::OnUpdate(Timestep ts)
	{
		UpdateViewMatrix();
	}

	void Camera::OnEvent(Event& e)
	{
	}

	void Camera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;

		switch (type)
		{
		case CStell::Camera::ProjectionType::Perspective:
			SetPerspective(m_PerspectiveFOV, m_PerspectiveNear, m_PerspectiveFar);
			break;
		case CStell::Camera::ProjectionType::Orthographic:
			SetOrthographic(m_OrthographicSize, m_OrthographicNear, m_OrthographicFar);
			break;
		default:
			break;
		}
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = Camera::ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		UpdateProjectionMatrix();
	}

	void Camera::SetPerspective(float fov, double nearClip, double farClip)
	{
		m_ProjectionType = Camera::ProjectionType::Perspective;
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		
		UpdateProjectionMatrix();
	}

	void Camera::UpdateOrientation(const glm::vec2& delta)
	{
		m_Orientation = glm::rotate(m_Orientation, glm::radians(-delta.x), m_UpDirection);
		m_Orientation = glm::rotate(m_Orientation, glm::radians(delta.y), glm::cross(m_UpDirection, m_Orientation));
	}

	void Camera::UpdateProjectionMatrix()
	{
		CSTELL_PROFILE_FUNCTION();

		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

		switch (m_ProjectionType)
		{
		case CStell::Camera::ProjectionType::Perspective:
		{
			m_ProjectionMatrix = glm::perspective<double>(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			break;
		}
		case CStell::Camera::ProjectionType::Orthographic:
		{
			float orthoLeft = -0.5f * m_AspectRatio * m_OrthographicSize;
			float orthoRight = 0.5f * m_AspectRatio * m_OrthographicSize;
			float orthoBottom = -0.5f * m_OrthographicSize;
			float orthoTop = 0.5f * m_OrthographicSize;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom,
				orthoTop, m_OrthographicNear, m_OrthographicFar);
			break;
		}
		default:
			break;
		}
	}

	void Camera::UpdateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::UpdateViewMatrix()
	{
		CSTELL_PROFILE_FUNCTION();

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDirection);;
		UpdateViewProjectionMatrix();
	}
}
