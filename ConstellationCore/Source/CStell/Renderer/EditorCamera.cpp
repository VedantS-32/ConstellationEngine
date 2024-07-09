#include "CStellpch.h"
#include "EditorCamera.h"

#include "CStell/Core/Input.h"
#include "CStell/Core/KeyCodes.h"
#include "CStell/Core/MouseButtonCodes.h"
#include "CStell/Core/InputMode.h"

namespace CStell
{
    EditorCamera::EditorCamera(float fov, double nearClip, double farClip)
    {
        m_ProjectionType = Camera::ProjectionType::Perspective;
        UpdateProjectionMatrix();
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.075f;
        m_InitialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(Mouse::CSTELL_BUTTON_RIGHT))
        {
            Input::SetInputMode(InputMode::CSETLL_CURSOR, CursorMode::CSTELL_CURSOR_DISABLED);
            m_MouseHidden = true;
            m_Moving = true;

            UpdateOrientation(delta);

            if (Input::IsKeyPressed(Key::CSTELL_KEY_W))
                m_Position += m_Speed * m_Orientation;
            if (Input::IsKeyPressed(Key::CSTELL_KEY_S))
                m_Position -= m_Speed * m_Orientation;
            if (Input::IsKeyPressed(Key::CSTELL_KEY_A))
                m_Position += m_Speed * glm::cross(m_UpDirection, m_Orientation);
            if (Input::IsKeyPressed(Key::CSTELL_KEY_D))
                m_Position -= m_Speed * glm::cross(m_UpDirection, m_Orientation);
            if (Input::IsKeyPressed(Key::CSTELL_KEY_Q))
                m_Position -= m_Speed * m_UpDirection;
            if (Input::IsKeyPressed(Key::CSTELL_KEY_E))
                m_Position += m_Speed * m_UpDirection;
        }
        else if (m_MouseHidden)
        {
            Input::SetInputMode(InputMode::CSETLL_CURSOR, CursorMode::CSTELL_CURSOR_NORMAL);
            m_MouseHidden = false;
            m_Moving = false;
        }

        UpdateViewMatrix();
    }

    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(CSTELL_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        if (Input::IsMouseButtonPressed(Mouse::CSTELL_BUTTON_RIGHT))
        {
            float delta = e.GetYOffset() * 0.005f;
            m_Speed += delta;
            m_Speed = std::clamp(m_Speed, 0.001f, 100.0f);
        }

        return false;
    }
}
