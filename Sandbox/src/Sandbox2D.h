#pragma once

#include <CStell.h>

class Sandbox2D : public CStell::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnUpdate(CStell::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(CStell::Event& e) override;

private:
	CStell::Ref<CStell::Shader> m_Shader;
	CStell::Ref<CStell::VertexArray> m_VertexArray;
	CStell::Ref<CStell::Texture2D> m_Texture;

	CStell::CameraController m_CameraController;

private:
	glm::vec4 m_SqaureColor{ 0.3f, 0.25f, 0.9f, 1.0f };
	glm::vec4 m_Tint{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 m_Translation{ 1.0f, 1.0f, 0.0f };
	float m_Tiling = 1.0f;
	float m_Rotation = 0.0f;
};