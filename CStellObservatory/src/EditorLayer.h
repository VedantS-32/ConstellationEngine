#pragma once

#include <CStell.h>

#include "Panels/SceneHierarchyPanel.h"

namespace CStell
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;

		Ref<Scene> m_ActiveScene;

		bool m_PrimaryCamera = true;
		EditorCamera m_EditorCamera;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	private:

		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}