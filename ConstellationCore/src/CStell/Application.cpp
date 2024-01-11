#include "Cstellpch.h"
#include "Application.h"

#include "CStell/Events/ApplicationEvent.h"

#include "GLFW/glfw3.h"

namespace CStell
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.25, 0.25, 0.25, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}