#pragma once

#include "CStell/Renderer/Camera.h"

namespace CStell
{
	class SceneCamera : public Camera
	{
	public:
		CSTELL_API SceneCamera();
		CSTELL_API virtual ~SceneCamera() = default;

		CSTELL_API virtual void OnUpdate(Timestep ts) override;
		CSTELL_API virtual void OnEvent(Event& e) override;

		CSTELL_API double GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		CSTELL_API void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; UpdateProjectionMatrix(); }
		CSTELL_API double GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		CSTELL_API void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; UpdateProjectionMatrix(); }
		CSTELL_API float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		CSTELL_API void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; UpdateProjectionMatrix(); }

		CSTELL_API float GetOrthographicSize() const { return m_OrthographicSize; }
		CSTELL_API void SetOrthographicSize(float size) { m_OrthographicSize = size; UpdateProjectionMatrix(); }
		CSTELL_API float GetOrthographicNearClip() const { return m_OrthographicNear; }
		CSTELL_API void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; UpdateProjectionMatrix(); }
		CSTELL_API float GetOrthographicFarClip() const { return m_OrthographicFar; }
		CSTELL_API void  SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; UpdateProjectionMatrix(); }
	};
}