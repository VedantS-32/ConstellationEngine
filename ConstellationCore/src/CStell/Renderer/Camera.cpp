#include "CStellpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CStell
{
	Camera::Camera(double left, double right, double bottom, double top, double zNear, double zFar)
		: m_ProjectionMatrix(glm::ortho<double>(left, right, bottom, top, zNear, zFar))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Camera::Camera(float FOV, float aspect, double zNear, double zFar)
		: m_ProjectionMatrix(glm::perspective<double>(glm::radians(FOV), aspect, zNear, zFar))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetOrthographic(double left, double right, double bottom, double top, double zNear, double zFar)
	{
		m_ProjectionMatrix = glm::ortho<double>(left, right, bottom, top, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetPerspective(float FOV, float aspect, double zNear, double zFar)
	{
		m_ProjectionMatrix = glm::perspective<double>(glm::radians(FOV), aspect, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
