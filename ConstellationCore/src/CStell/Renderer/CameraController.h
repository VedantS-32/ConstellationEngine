#pragma once

#include "CStell/Renderer/Camera.h"
#include "CStell/Core/Timestep.h"

#include "CStell/Events/ApplicationEvent.h"
#include "CStell/Events/MouseEvent.h"

namespace CStell
{
	class CameraController
	{
	public:
		CameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		Camera m_Camera;

		bool m_Rotate;
		glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
		float m_CameraSpeed = 1.0f;
	};
}