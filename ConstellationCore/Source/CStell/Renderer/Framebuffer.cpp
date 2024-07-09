#include "CStellpch.h"
#include "FrameBuffer.h"

#include "CStell/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace CStell
{
	CStell::Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}