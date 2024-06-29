#pragma once

#include "Camera.h"

namespace CStell
{
	class EditorCamera : public Camera
	{
	public:
		CSTELL_API EditorCamera() = default;
		CSTELL_API EditorCamera(float fov, double nearClip, double farClip);

		CSTELL_API virtual void OnUpdate(Timestep ts) override;
		CSTELL_API virtual void OnEvent(Event& e) override;

		CSTELL_API bool IsMoving() const { return m_Moving; }

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);

	private:
		glm::vec2 m_InitialMousePosition{ 0.0f };
		float m_Speed = 0.025f;
		bool m_MouseHidden = false;
		bool m_Moving = false;
	};
}