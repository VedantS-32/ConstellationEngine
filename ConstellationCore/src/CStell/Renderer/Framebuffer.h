#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/Stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		CSTELL_API FramebufferTextureSpecification() = default;
		CSTELL_API FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		CSTELL_API FramebufferAttachmentSpecification() = default;
		CSTELL_API FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		CSTELL_API virtual ~Framebuffer() = default;

		CSTELL_API virtual void Bind() = 0;
		CSTELL_API virtual void UnBind() = 0;

		CSTELL_API virtual void Resize(uint32_t width, uint32_t height) = 0;
		CSTELL_API virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		CSTELL_API virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		CSTELL_API virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		CSTELL_API virtual const FramebufferSpecification& GetSpecification() const = 0;

		CSTELL_API static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}