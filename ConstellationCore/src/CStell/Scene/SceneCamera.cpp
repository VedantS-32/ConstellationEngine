#include "CStellpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CStell
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
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
		float orthoLeft = -0.5f * m_AspectRatio * m_OrthographicSize;
		float orthoRight = 0.5f * m_AspectRatio * m_OrthographicSize;
		float orthoBottom = -0.5f * m_OrthographicSize;
		float orthoTop = 0.5f * m_OrthographicSize;

		m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom,
			orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}
