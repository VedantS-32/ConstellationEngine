#include "CStellpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace CStell
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, size);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

        return nullptr;
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t bindingPoint)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(bindingPoint);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

        return nullptr;
    }
}
