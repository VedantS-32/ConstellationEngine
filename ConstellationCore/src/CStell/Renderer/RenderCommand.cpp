#include "CStellpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CStell
{
	RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

	inline RendererAPI& RenderCommand::Get()
	{
		return *s_RendererAPI;
	}
}