#pragma once

#include "Core.h"

#include "Window.h"
#include "CStell/LayerStack.h"
#include "Events/Event.h"
#include "CStell/Events/ApplicationEvent.h"

#include "CStell/Core/Timestep.h"

#include "CStell/ImGui/ImGuiLayer.h"

namespace CStell
{
	
	class CSTELL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}