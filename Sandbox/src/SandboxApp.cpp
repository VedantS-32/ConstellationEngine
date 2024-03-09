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
		: Layer("Scene"), m_CameraController(1280.0f / 720.0f)
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

		auto shader = m_ShaderLibrary.Load("asset/shader/Texture.glsl");

		m_Texture = CStell::Texture2D::Create("asset/texture/CStell.png");
		m_Texture->Bind();

		std::dynamic_pointer_cast<CStell::OpenGLShader>(shader)->Bind();
		std::dynamic_pointer_cast<CStell::OpenGLShader>(shader)->SetUniform1i("u_Texture", 0);
	}

	void OnUpdate(CStell::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		CStell::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		CStell::RenderCommand::Clear();

		CStell::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		auto shader = m_ShaderLibrary.Get("Texture");
		shader->Bind();

		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.25f, y * 0.25f, 0.0f);
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos) * scale;
				CStell::Renderer::Submit(shader, m_VertexArray, modelMatrix);
			}
		}

		CStell::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Info");
		ImGui::Text("Texture");
		ImGui::End();
	}

	void OnEvent(CStell::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	CStell::ShaderLibrary m_ShaderLibrary;
	CStell::Ref<CStell::Shader> m_Shader;
	CStell::Ref<CStell::VertexArray> m_VertexArray;
	CStell::Ref<CStell::Texture2D> m_Texture;

	CStell::CameraController m_CameraController;
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