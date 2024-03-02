#include "CStellpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace CStell
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: CSTELL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
        }

        CSTELL_CORE_ASSERT(false, "Unknown RendererAPI!");

        return nullptr;
    }
}
