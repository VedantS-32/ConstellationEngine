#include "CStellpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace CStell
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_QuadData;

	void Renderer2D::Init()
	{
		s_QuadData = new Renderer2DStorage;

		s_QuadData->QuadVertexArray = VertexArray::Create();

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

		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);
		s_QuadData->QuadVertexArray->AddVertexBuffer(vertexBuffer);
		s_QuadData->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_QuadData->FlatColorShader = Shader::Create("asset/shader/Flat2DShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_QuadData;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		std::dynamic_pointer_cast<CStell::OpenGLShader>(s_QuadData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<CStell::OpenGLShader>(s_QuadData->FlatColorShader)->SetUniformMat4f("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<CStell::OpenGLShader>(s_QuadData->FlatColorShader)->SetUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<CStell::OpenGLShader>(s_QuadData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<CStell::OpenGLShader>(s_QuadData->FlatColorShader)->SetUniform4f("u_Color", color);

		s_QuadData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color)
	{
	}
}
