#pragma once

#include "CStell/Renderer/Framebuffer.h"

namespace CStell
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}