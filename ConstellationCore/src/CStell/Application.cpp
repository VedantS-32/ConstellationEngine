#include "Cstellpch.h"
#include "Application.h"

#include "CStell/Events/ApplicationEvent.h"
#include "CStell/Log.h"

namespace CStell
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			CSTELL_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			CSTELL_TRACE(e);
		}
		while (true);
	}
}