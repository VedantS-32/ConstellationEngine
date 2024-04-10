#pragma once

#include <CStell.h>

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
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_GrassField;
		Ref<SubTexture2D> m_Water;
		Ref<SubTexture2D> m_SignBoardTex;

		CameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Entity m_Square;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = false;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	private:
		glm::vec4 m_SquareColor{ 0.3f, 0.25f, 0.9f, 1.0f };
		glm::vec4 m_Tint{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 m_Translation{ 1.0f, 1.0f, 0.0f };
		float m_Tiling = 1.0f;
		float m_Rotation = 0.0f;
	};
}