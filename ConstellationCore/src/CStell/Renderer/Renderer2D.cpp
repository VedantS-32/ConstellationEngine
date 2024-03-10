#include "CStellpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>

namespace CStell
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture> WhiteTexture;
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

		s_QuadData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_QuadData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_QuadData->TextureShader = Shader::Create("asset/shader/Texture.glsl");
		s_QuadData->TextureShader->Bind();
		s_QuadData->TextureShader->Set1i("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_QuadData;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_QuadData->TextureShader->Bind();
		s_QuadData->TextureShader->SetMat4f("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tiling)
	{
		s_QuadData->TextureShader->Set4f("u_Tint", color);
		s_QuadData->WhiteTexture->Bind();
		s_QuadData->TextureShader->Set1f("u_Tiling", tiling);

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData->TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec4& tint, float tiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec4& tint, float tiling)
	{
		s_QuadData->TextureShader->Set4f("u_Tint", tint);
		s_QuadData->TextureShader->Set1f("u_Tiling", tiling);
		texture->Bind();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData->TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData->QuadVertexArray);

	}
}
