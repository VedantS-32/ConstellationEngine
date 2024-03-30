#include "CStellpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>

namespace CStell
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Query GPU limit

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 == White Texture
	};

	static Renderer2DData s_QuadData;

	void Renderer2D::Init()
	{
		CSTELL_PROFILE_FUNCTION();

		s_QuadData.QuadVertexBufferBase = new QuadVertex[s_QuadData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_QuadData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_QuadData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_QuadData.QuadVertexArray = VertexArray::Create();

		s_QuadData.QuadVertexBuffer = VertexBuffer::Create(s_QuadData.MaxQuads * sizeof(QuadVertex));
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_QuadData.MaxIndices);

		s_QuadData.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float, "a_TexIndex" },
				{ ShaderDataType::Float, "a_TilingFactor" }
			});

		s_QuadData.QuadVertexArray->AddVertexBuffer(s_QuadData.QuadVertexBuffer);
		s_QuadData.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;

		s_QuadData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_QuadData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_QuadData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_QuadData.MaxTextureSlots; i++)
			samplers[i] = i;

		s_QuadData.TextureShader = Shader::Create("asset/shader/Texture.glsl");
		s_QuadData.TextureShader->Bind();
		s_QuadData.TextureShader->Set1iArray("u_Texture", samplers, s_QuadData.MaxTextureSlots);

		// Setting all texture slots to 0
		s_QuadData.TextureSlots[0] = s_QuadData.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		CSTELL_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		CSTELL_PROFILE_FUNCTION();

		s_QuadData.TextureShader->Bind();
		s_QuadData.TextureShader->SetMat4f("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_QuadData.QuadIndexCount = 0;
		s_QuadData.QuadVertexBufferPtr = s_QuadData.QuadVertexBufferBase;
		s_QuadData.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		CSTELL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_QuadData.QuadVertexBufferPtr - (uint8_t*)s_QuadData.QuadVertexBufferBase;
		s_QuadData.QuadVertexBuffer->SetData(s_QuadData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < s_QuadData.TextureSlotIndex; i++)
			s_QuadData.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray, s_QuadData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		CSTELL_PROFILE_FUNCTION();

		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		s_QuadData.QuadVertexBufferPtr->Position = position;
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = texIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = texIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = texIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = texIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadIndexCount += 6;

		/*s_QuadData.WhiteTexture->Bind();
		s_QuadData.TextureShader->Set1f("u_Tiling", tilingFactor);

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData.TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_QuadData.TextureSlotIndex; i++)
		{
			if (*s_QuadData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		s_QuadData.QuadVertexBufferPtr->Position = position;
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadIndexCount += 6;


		//s_QuadData.TextureShader->Set4f("u_Tint", tint);
		s_QuadData.TextureShader->Set1f("u_Tiling", tilingFactor);
		texture->Bind();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData.TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		CSTELL_PROFILE_FUNCTION();

		s_QuadData.TextureShader->Set4f("u_Tint", color);
		s_QuadData.WhiteTexture->Bind();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData.TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
		}

		s_QuadData.QuadVertexBufferPtr->Position = position;
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_QuadData.QuadVertexBufferPtr->Color = color;
		s_QuadData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_QuadData.QuadVertexBufferPtr++;

		s_QuadData.QuadIndexCount += 6;

#if OLD_PATH
		s_QuadData.TextureShader->Set4f("u_Tint", tint);
		s_QuadData.TextureShader->Set1f("u_Tiling", tilingFactor);
		texture->Bind();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
								glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
								glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		s_QuadData.TextureShader->SetMat4f("u_ModelMatrix", modelMatrix);

		s_QuadData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray);
#endif
	}
}
