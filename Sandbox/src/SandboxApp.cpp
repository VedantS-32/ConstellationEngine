#include <CStell.h>
#include <CStellpch.h>

#include <glm/glm.hpp>

class SceneLayer : public CStell::Layer
{
public:
	SceneLayer()
		: Layer("Scene"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f, -100.0f, 100.0f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(CStell::VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<CStell::VertexBuffer> vertexBuffer;
		std::shared_ptr<CStell::IndexBuffer> indexBuffer;

		vertexBuffer.reset(CStell::VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(CStell::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		CStell::BufferLayout layout = {
			{ CStell::ShaderDataType::Float3, "a_Position"},
			{ CStell::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjectionMatrix;

		out vec4 v_Color;
		
		void main()
		{
			gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			v_Color = a_Color;
		};
	)";

		std::string fragmentSrc = R"(
		#version 330 core
		
		in vec4 v_Color;

		out vec4 fragColor;
		
		void main()
		{
			fragColor = v_Color;
		};
	)";

		m_Shader.reset(new CStell::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{
		if (CStell::Input::IsKeyPressed(CSTELL_KEY_W))
			m_CameraPosition.y += m_CameraSpeed;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed;

		if (CStell::Input::IsKeyPressed(CSTELL_KEY_D))
			m_CameraPosition.x += m_CameraSpeed;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed;

		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		CStell::Renderer::BeginScene(m_Camera);

		m_Shader->Bind();
		CStell::Renderer::Submit(m_Shader, m_VertexArray);

		CStell::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(CStell::Event& event) override
	{
	}

private:
	std::shared_ptr<CStell::Shader> m_Shader;
	std::shared_ptr<CStell::VertexArray> m_VertexArray;

	CStell::Camera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 0.01f;
};

class Sandbox : public CStell::Application
{

public:
	Sandbox()
	{
		PushLayer(new SceneLayer());
	}

	~Sandbox()
	{

	}

};


CStell::Application* CStell::CreateApplication()
{
	CSTELL_INFO("Creating CStell Application!");
	return new Sandbox();
}