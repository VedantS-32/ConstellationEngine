#pragma once

#include "CStell/Core/Core.h"

namespace CStell
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		// FrameBufferFormat
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}