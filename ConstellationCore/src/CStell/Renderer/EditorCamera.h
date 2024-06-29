#pragma once

#include "Camera.h"

namespace CStell
{
	class CSTELL_API EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, double nearClip, double farClip);

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		bool IsMoving() const { return m_Moving; }

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);

	private:
		glm::vec2 m_InitialMousePosition{ 0.0f };
		float m_Speed = 0.025f;
		bool m_MouseHidden = false;
		bool m_Moving = false;
	};
}