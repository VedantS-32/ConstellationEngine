#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Scene"), m_Camera(45.0f, 0.1f, 10000.0f)
{
}

void Sandbox2D::OnAttach()
{
    CStell::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = CStell::Framebuffer::Create(fbSpec);
	m_Texture = CStell::Texture2D::Create("asset/texture/CStell.png");
	m_SpriteSheet = CStell::Texture2D::Create("asset/gameTest0/tiny-16.png");
	m_TextureMap['#'] = CStell::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 9}, {16, 16});
	m_TextureMap['W'] = CStell::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 6}, {16, 16});
    m_SignBoardTex = CStell::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 16, 16 });
}

void Sandbox2D::OnUpdate(CStell::Timestep ts)
{
	CSTELL_PROFILE_FUNCTION();

	// Update
	m_Camera.OnUpdate(ts);

	// Render
	CStell::Renderer2D::ResetStats();

	{
		CSTELL_PROFILE_SCOPE("Renderer Preparation");
		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.f;

		CSTELL_PROFILE_SCOPE("Renderer Draw");
		CStell::Renderer2D::BeginScene(m_Camera);
		CStell::Renderer2D::DrawQuad({ 0.5f, 0.5f, -0.1f }, { 1.0f, 1.0f }, m_SquareColor);
		CStell::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Rotation, m_Texture, m_Tint, m_Tiling);
		CStell::Renderer2D::DrawRotatedQuad({ -2.0f, -1.0f }, { 1.0f, 1.0f }, rotation, m_Texture, m_Tint, m_Tiling);
		CStell::Renderer2D::DrawQuad({ 3.0f, 3.0f }, { 1.0f, 1.0f }, m_Texture, m_Tint);
		CStell::Renderer2D::DrawRotatedQuad({ 5.0f, 3.0f }, { 1.0f, 1.0f }, 45.0f, m_SquareColor);
		CStell::Renderer2D::DrawQuad(m_Translation, { 2.0f, 4.0f }, { 0.9f, 0.5f, 0.5f, 1.0f });
		CStell::Renderer2D::EndScene();

		CStell::Renderer2D::BeginScene(m_Camera);
		for (float x = -5.0f; x < 5.0f; x+=0.25f)
			for (float y = -5.0f; y < 5.0f; y+=0.25f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.f, 0.25f, (y + 5.0f) / 10.0f, 0.75f };
				CStell::Renderer2D::DrawQuad({ x, y }, { 0.20f, 0.20f }, color);
			}
		CStell::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Info");
    ImGui::Text("2D Renderer");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::ColorEdit4("Tint", glm::value_ptr(m_Tint));
    ImGui::SliderFloat3("Translation", glm::value_ptr(m_Translation), -1.0f, 10.0f);
    ImGui::SliderFloat("Tiling", &m_Tiling, 1.0f, 10.0f);
    ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
    uint32_t textureID = m_Texture->GetRendererID();
    ImGui::Image((void*)(uint64_t)textureID, ImVec2{ 100, 100 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();

    auto stats = CStell::Renderer2D::GetStats();
    ImGui::Begin("Renderer2D Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

}

void Sandbox2D::OnEvent(CStell::Event& e)
{
	m_Camera.OnEvent(e);
}
