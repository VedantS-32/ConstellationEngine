#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Scene"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnUpdate(CStell::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	CStell::RenderCommand::Clear();

	CStell::Renderer2D::BeginScene(m_CameraController.GetCamera());
	CStell::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SqaureColor);
	CStell::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Info");
	ImGui::Text("2D Renderer");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqaureColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(CStell::Event& e)
{
	m_CameraController.OnEvent(e);
}
