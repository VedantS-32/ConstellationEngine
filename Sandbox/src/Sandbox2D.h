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

	CStell::CameraController m_CameraController;

private:
	glm::vec4 m_SqaureColor{ 0.3f, 0.25f, 0.9f, 1.0f };
};