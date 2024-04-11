#include "CStellpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CStell
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetPerspective(float FOV, double nearClip, double farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = FOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{

		switch (m_ProjectionType)
		{
		case CStell::SceneCamera::ProjectionType::Perspective:
		{
			m_ProjectionMatrix = glm::perspective<double>(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			break;
		}
		case CStell::SceneCamera::ProjectionType::Orthographic:
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

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		
		switch (type)
		{
		case CStell::SceneCamera::ProjectionType::Perspective:
			SetPerspective(m_PerspectiveFOV, m_PerspectiveNear, m_PerspectiveFar);
			break;
		case CStell::SceneCamera::ProjectionType::Orthographic:
			SetOrthographic(m_OrthographicSize, m_OrthographicNear, m_OrthographicFar);
			break;
		default:
			break;
		}
	}
}
