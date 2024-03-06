#include <CStell.h>
#include <CStellpch.h>

#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SceneLayer : public CStell::Layer
{
public:
	SceneLayer()
		: Layer("Scene"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f, -100.0f, 100.0f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(CStell::VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		CStell::Ref<CStell::VertexBuffer> vertexBuffer;
		CStell::Ref<CStell::IndexBuffer> indexBuffer;

		vertexBuffer.reset(CStell::VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(CStell::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		CStell::BufferLayout layout = {
			{ CStell::ShaderDataType::Float3, "a_Position"},
			{ CStell::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(CStell::Shader::Create("asset/shader/Texture.glsl"));

		m_Texture = CStell::Texture2D::Create("asset/texture/CStell.png");
		m_Texture->Bind();

		std::dynamic_pointer_cast<CStell::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<CStell::OpenGLShader>(m_Shader)->SetUniform1i("u_Texture", 0);
	}

	void OnUpdate(CStell::Timestep ts) override
	{
		//CSTELL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (CStell::Input::IsKeyPressed(CSTELL_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * ts;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (CStell::Input::IsKeyPressed(CSTELL_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * ts;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		if (CStell::Input::IsKeyPressed(CSTELL_KEY_Q))
			m_SquarePosition.y += m_SquareSpeed * ts;

		else if (CStell::Input::IsKeyPressed(CSTELL_KEY_E))
			m_SquarePosition.y -= m_SquareSpeed * ts;

		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		CStell::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.25f, y * 0.25f, 0.0f);
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos) * scale;
				CStell::Renderer::Submit(m_Shader, m_VertexArray, modelMatrix);
			}
		}

		CStell::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Info");
		ImGui::Text("Texture");
		ImGui::End();

		ImGui::Begin("Square Color");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(CStell::Event& event) override
	{
	}

private:
	CStell::Ref<CStell::Shader> m_Shader;
	CStell::Ref<CStell::VertexArray> m_VertexArray;
	CStell::Ref<CStell::Texture2D> m_Texture;

	CStell::Camera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;
	glm::vec3 m_SquarePosition;
	float m_SquareSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.3, 0.2, 0.8 };
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