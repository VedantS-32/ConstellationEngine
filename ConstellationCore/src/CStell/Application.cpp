#include "Cstellpch.h"
#include "Application.h"

#include "Input.h"
#include "Renderer/Renderer.h"

namespace CStell
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CSTELL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = { 
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		out vec4 v_Color;
		
		void main()
		{
			gl_Position = vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			/*if (e.Handled)
				break;*/
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}