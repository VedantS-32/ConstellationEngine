#include "EditorLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "CStell/Scene/SceneSerializer.h"
#include "CStell/Utils/PlatformUtils.h"
#include "CStell/Math/Math.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

namespace CStell
{
    extern const std::filesystem::path s_AssetPath;

    static void SetImGuiHovered(bool active)
    {
        ImGuiStyle& style = ImGui::GetStyle();

        if (active)
        {
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.6f, 0.78f, 1.0f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.89f, 0.34f, 0.47f, 1.0f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.42f, 0.57f, 0.55f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.23f, 0.23f, 1.0f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9f, 0.34f, 0.47f, 1.0f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.3f, 0.46f, 0.53f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.3f, 0.42f, 1.0f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.88f, 0.46f, 0.6f, 1.0f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.73f, 0.69f, 0.88f, 0.54f);
        }
        else
        {
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.6f, 0.78f, 0.0f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.89f, 0.34f, 0.47f, 0.0f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.42f, 0.57f, 0.0f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.23f, 0.23f, 0.0f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9f, 0.34f, 0.47f, 0.0f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.3f, 0.46f, 0.0f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.3f, 0.42f, 0.0f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.88f, 0.46f, 0.6f, 0.0f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.73f, 0.69f, 0.88f, 0.0f);
        }
    }

    EditorLayer::EditorLayer()
        : Layer("Scene")
    {
    }

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_EditorCamera = EditorCamera(45.0f, 0.1f, 10000.0f);
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

#if 1
        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize("asset/scene/scene3.cstell");
#endif
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        CSTELL_PROFILE_FUNCTION();

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Render
        Renderer2D::ResetStats();

        CSTELL_PROFILE_SCOPE("Renderer Preparation");
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        RenderCommand::Clear();

        // Clear our entity ID attachment to -1
        m_Framebuffer->ClearAttachment(1, -1);

        CSTELL_PROFILE_SCOPE("Renderer Draw");


        // Update Scene
        if (m_EditorCamera.IsMoving())
            SetImGuiHovered(false);
        else
            SetImGuiHovered(true);

        m_EditorCamera.OnUpdate(ts);
        m_ActiveScene->OnUpdateRuntime(ts);
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

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
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        if (opt_fullscreen)
        {
            //m_ViewportSize = { viewport->WorkSize.x, viewport->WorkSize.y };
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
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        minWinSizeX = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportSize.x, viewportSize.y };
        if (m_ViewportSize != *((glm::vec2*)&viewportSize) && viewportSize.x > 0 && viewportSize.y > 0)
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ViewportSize = { viewportSize.x, viewportSize.y };
        }

        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                OpenScene(std::filesystem::path(s_AssetPath) / path);
            }
            ImGui::EndDragDropTarget();
        }

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Runtime Camera Entity
            //auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
            //const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            //const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
            //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            // Editor Camera
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


            // Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::CSTELL_KEY_LEFT_CONTROL);
            float snapValue = 0.5f; // Snap to 0.5m for translation/rotation
            // Snap to 10 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 10.0f;

            float snapValues[] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing() && m_GizmoType != -1)
            {
                glm::vec3 translation{ 0.0f }, rotation{ 0.0f }, scale{ 1.0f };
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(CSTELL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(CSTELL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }
    
    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::CSTELL_KEY_LEFT_CONTROL) || Input::IsKeyPressed(Key::CSTELL_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(Key::CSTELL_KEY_LEFT_SHIFT) || Input::IsKeyPressed(Key::CSTELL_KEY_RIGHT_SHIFT);

        switch (e.GetKeyCode())
        {
            case Key::CSTELL_KEY_N:
            {
                if (control)
                    NewScene();
                break;
            }
            case Key::CSTELL_KEY_O:
            {
                if (control)
                    OpenScene();
                break;
            }
            case Key::CSTELL_KEY_S:
            {
                if (control && shift)
                    SaveSceneAs();
                break;
            }
            default:
                break;
        }

        if(!m_EditorCamera.IsMoving())
        {
            switch (e.GetKeyCode())
            {
                // Gizmo
                case Key::CSTELL_KEY_Q:
                {
                    m_GizmoType = -1;
                    break;
                }
                case Key::CSTELL_KEY_W:
                {
                    m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                    break;
                }
                case Key::CSTELL_KEY_E:
                {
                    m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                    break;
                }
                case Key::CSTELL_KEY_R:
                {
                    m_GizmoType = ImGuizmo::OPERATION::SCALE;
                    break;
                }
                default:
                    break;
            }
        }

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mouse::CSTELL_BUTTON_LEFT)
        {
            if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::CSTELL_KEY_LEFT_ALT))
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }

        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("CStell Scene (*.cstell)\0*.cstell\0");
        if (!filepath.empty())
        {
            OpenScene(filepath);
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize(path.string());
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("CStell Scene (*.cstell)\0*.cstell\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
}
