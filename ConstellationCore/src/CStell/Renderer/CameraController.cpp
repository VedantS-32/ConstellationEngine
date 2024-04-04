#include "CStellpch.h"
#include "CameraController.h"

#include "CStell/Core/Input.h"
#include "CStell/Core/KeyCodes.h"

namespace CStell
{
	CameraController::CameraController(float aspectRatio, bool rotate)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -100.0f, 100.0f), m_Rotate(rotate)
	{
	}

	void CameraController::OnUpdate(Timestep ts)

	{
		CSTELL_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(CSTELL_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * ts;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (Input::IsKeyPressed(CSTELL_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * ts;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
	}

	void CameraController::OnEvent(Event& e)
	{
		CSTELL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CSTELL_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CSTELL_BIND_EVENT_FN(CameraController::OnWindowResize));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		CSTELL_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetOrthographic(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -100.0f, 100.0f);
		m_CameraSpeed = m_ZoomLevel;
		return false;
	}

	bool CameraController::OnWindowResize(WindowResizeEvent& e)
	{
		CSTELL_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetOrthographic(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -100.0f, 100.0f);
		return false;
	}
}
