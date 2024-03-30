#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Scene"), m_CameraController(1280.0f / 720.0f)
{
	m_Texture = CStell::Texture2D::Create("asset/texture/CStell.png");
}

void Sandbox2D::OnUpdate(CStell::Timestep ts)
{
	CSTELL_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		CSTELL_PROFILE_SCOPE("Renderer Preparation");
		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();
	}

	{
		CSTELL_PROFILE_SCOPE("Renderer Draw");
		CStell::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CStell::Renderer2D::DrawQuad({ 0.5f, 0.5f, -0.1f }, { 1.0f, 1.0f }, m_SqaureColor);
		CStell::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, m_Texture, m_Tint, m_Tiling);
		CStell::Renderer2D::DrawQuad({ 3.0f, 3.0f }, { 1.0f, 1.0f }, m_Texture, m_Tint);
		CStell::Renderer2D::DrawQuad(m_Translation, { 2.0f, 4.0f }, { 0.9f, 0.5f, 0.5f, 1.0f });
		CStell::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CSTELL_PROFILE_FUNCTION();

	ImGui::Begin("Info");
	ImGui::Text("2D Renderer");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqaureColor));
	ImGui::ColorEdit4("Tint", glm::value_ptr(m_Tint));
	ImGui::SliderFloat3("Translation", glm::value_ptr(m_Translation), -1.0f, 10.0f);
	ImGui::SliderFloat("Tiling", &m_Tiling, 1.0f, 10.0f);
	//ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(CStell::Event& e)
{
	m_CameraController.OnEvent(e);
}
