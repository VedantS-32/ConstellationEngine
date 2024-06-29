#pragma once

#include "CStell/Renderer/Camera.h"

namespace CStell
{
	class CSTELL_API SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		double GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; UpdateProjectionMatrix(); }
		double GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; UpdateProjectionMatrix(); }
		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; UpdateProjectionMatrix(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; UpdateProjectionMatrix(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; UpdateProjectionMatrix(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void  SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; UpdateProjectionMatrix(); }
	};
}