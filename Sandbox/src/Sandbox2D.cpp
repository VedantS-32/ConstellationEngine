#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Scene"), m_CameraController(1280.0f / 720.0f)
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
	m_SignBoardTex = CStell::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2, 1}, {16, 16});

}

void Sandbox2D::OnUpdate(CStell::Timestep ts)
{
	CSTELL_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	CStell::Renderer2D::ResetStats();

	{
		CSTELL_PROFILE_SCOPE("Renderer Preparation");
        m_Framebuffer->Bind();
		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.f;

		CSTELL_PROFILE_SCOPE("Renderer Draw");
		CStell::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CStell::Renderer2D::DrawQuad({ 0.5f, 0.5f, -0.1f }, { 1.0f, 1.0f }, m_SquareColor);
		CStell::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Rotation, m_Texture, m_Tint, m_Tiling);
		CStell::Renderer2D::DrawRotatedQuad({ -2.0f, -1.0f }, { 1.0f, 1.0f }, rotation, m_Texture, m_Tint, m_Tiling);
		CStell::Renderer2D::DrawQuad({ 3.0f, 3.0f }, { 1.0f, 1.0f }, m_Texture, m_Tint);
		CStell::Renderer2D::DrawRotatedQuad({ 5.0f, 3.0f }, { 1.0f, 1.0f }, 45.0f, m_SquareColor);
		CStell::Renderer2D::DrawQuad(m_Translation, { 2.0f, 4.0f }, { 0.9f, 0.5f, 0.5f, 1.0f });
		CStell::Renderer2D::EndScene();

		CStell::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float x = -5.0f; x < 5.0f; x+=0.25f)
			for (float y = -5.0f; y < 5.0f; y+=0.25f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.f, 0.25f, (y + 5.0f) / 10.0f, 0.75f };
				CStell::Renderer2D::DrawQuad({ x, y }, { 0.20f, 0.20f }, color);
			}
		CStell::Renderer2D::EndScene();
        m_Framebuffer->UnBind();
	}
}

void Sandbox2D::OnImGuiRender()
{
    CSTELL_PROFILE_FUNCTION();

    static bool dockSpaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) { CStell::Application::Get().Close(); }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Info");
    ImGui::Text("2D Renderer");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::ColorEdit4("Tint", glm::value_ptr(m_Tint));
    ImGui::SliderFloat3("Translation", glm::value_ptr(m_Translation), -1.0f, 10.0f);
    ImGui::SliderFloat("Tiling", &m_Tiling, 1.0f, 10.0f);
    ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
    uint32_t textureID = m_Texture->GetRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 100, 100 });
    ImGui::End();

    ImGui::Begin("Viewport");
    textureID = m_Framebuffer->GetColorAttachmentRendererID();
    CSTELL_INFO(textureID);
    ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
    ImGui::End();

    auto stats = CStell::Renderer2D::GetStats();
    ImGui::Begin("Renderer2D Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnEvent(CStell::Event& e)
{
	m_CameraController.OnEvent(e);
}
