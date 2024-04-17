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
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
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

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_QuadData;

	void Renderer2D::Init()
	{
		CSTELL_PROFILE_FUNCTION();

		s_QuadData.QuadVertexArray = VertexArray::Create();

		s_QuadData.QuadVertexBuffer = VertexBuffer::Create(s_QuadData.MaxQuads * sizeof(QuadVertex));
		s_QuadData.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float, "a_TexIndex" },
				{ ShaderDataType::Float, "a_TilingFactor" }
			});

		s_QuadData.QuadVertexArray->AddVertexBuffer(s_QuadData.QuadVertexBuffer);

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

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_QuadData.MaxIndices);
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

		s_QuadData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_QuadData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
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

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		CSTELL_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjectionMatrix();

		s_QuadData.TextureShader->Bind();
		s_QuadData.TextureShader->SetMat4f("u_ViewProjectionMatrix", viewProj);

		StartBatch();

	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CSTELL_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjectionMatrix() * glm::inverse(transform);

		s_QuadData.TextureShader->Bind();
		s_QuadData.TextureShader->SetMat4f("u_ViewProjectionMatrix", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		CSTELL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_QuadData.QuadIndexCount = 0;
		s_QuadData.QuadVertexBufferPtr = s_QuadData.QuadVertexBufferBase;

		s_QuadData.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::Flush()
	{
		if (s_QuadData.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_QuadData.QuadVertexBufferPtr - (uint8_t*)s_QuadData.QuadVertexBufferBase);
		s_QuadData.QuadVertexBuffer->SetData(s_QuadData.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_QuadData.TextureSlotIndex; i++)
			s_QuadData.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_QuadData.QuadVertexArray, s_QuadData.QuadIndexCount);
		s_QuadData.Stats.DrawCalls++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		CSTELL_PROFILE_FUNCTION();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
		
		DrawQuad(modelMatrix, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		DrawQuad(modelMatrix, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tint, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			if (s_QuadData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = modelMatrix * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = transform * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			if (s_QuadData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = transform * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = modelMatrix * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			if (s_QuadData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = modelMatrix * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tint, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint, float tilingFactor)
	{
		CSTELL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_QuadData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

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
			if (s_QuadData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_QuadData.TextureSlotIndex;
			s_QuadData.TextureSlots[s_QuadData.TextureSlotIndex] = texture;
			s_QuadData.TextureSlotIndex++;
		}

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_QuadData.QuadVertexBufferPtr->Position = modelMatrix * s_QuadData.QuadVertexPositions[i];
			s_QuadData.QuadVertexBufferPtr->Color = color;
			s_QuadData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_QuadData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_QuadData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_QuadData.QuadVertexBufferPtr++;
		}

		s_QuadData.QuadIndexCount += 6;

		s_QuadData.Stats.QuadCount++;

	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_QuadData.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_QuadData.Stats, 0, sizeof(Statistics));
	}
}
