#include "CStellpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CStell
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CSTELL_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CSTELL_CORE_ASSERT(status, "Failed to initialize Glad");

		CSTELL_CORE_INFO("OpenGL Info:");
		CSTELL_CORE_INFO("    Vendor: {0}", ((char*)glGetString(GL_VENDOR)));
		CSTELL_CORE_INFO("    Renderer: {0}", ((char*)glGetString(GL_RENDERER)));
		CSTELL_CORE_INFO("    Version: {0}", ((char*)glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}