#include "EditorLayer.h"

#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace CStell
{

    EditorLayer::EditorLayer()
        : Layer("Scene"), m_CameraController(1280.0f / 720.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;

        m_ActiveScene = CreateRef<Scene>();

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
        m_CameraEntity.AddComponent<CameraComponent>();

        m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
        auto& cam =  m_SecondCamera.AddComponent<CameraComponent>();

        cam.Primary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
                //GetComponent<TransformComponent>();
                CSTELL_INFO("Created CameraController!");
            }

            void OnDestroy()
            {
            }

            void OnUpdate(Timestep ts)
            {
                auto& translation = GetComponent<TransformComponent>().Translation;

                if (Input::IsKeyPressed(CSTELL_KEY_W))
                    translation.y += m_CameraSpeed * ts;

                else if (CStell::Input::IsKeyPressed(CSTELL_KEY_S))
                    translation.y -= m_CameraSpeed * ts;

                if (Input::IsKeyPressed(CSTELL_KEY_D))
                    translation.x += m_CameraSpeed * ts;

                else if (CStell::Input::IsKeyPressed(CSTELL_KEY_A))
                    translation.x -= m_CameraSpeed * ts;
            }

        private:
            float m_CameraSpeed = 3.0f;
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_Square = m_ActiveScene->CreateEntity("Square");
        m_Square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        m_Framebuffer = Framebuffer::Create(fbSpec);
        m_Texture = Texture2D::Create("asset/texture/CStell.png");
        m_SpriteSheet = Texture2D::Create("asset/gameTest0/tiny-16.png");
        m_TextureMap['#'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 9 }, { 16, 16 });
        m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 6 }, { 16, 16 });
        m_SignBoardTex = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 16, 16 });

    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        CSTELL_PROFILE_FUNCTION();

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Update
        if(m_ViewportFocused)
            m_CameraController.OnUpdate(ts);

        // Render
        Renderer2D::ResetStats();

        CSTELL_PROFILE_SCOPE("Renderer Preparation");
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        RenderCommand::Clear();

        CSTELL_PROFILE_SCOPE("Renderer Draw");

        //Renderer2D::BeginScene(m_CameraController.GetCamera());

        // Update Scene
        m_ActiveScene->OnUpdate(ts);

        //Renderer2D::EndScene();

        m_Framebuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender()
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
            if (ImGui::BeginMenu("Constellation Engine"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportSize) && viewportSize.x > 0 && viewportSize.y > 0)
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ViewportSize = { viewportSize.x, viewportSize.y };

            m_CameraController.OnResize(viewportSize.x, viewportSize.y);
        }

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::PopStyleVar();
        ImGui::End();

        auto stats = Renderer2D::GetStats();
        ImGui::Begin("Renderer2D Stats");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);
    }
}
