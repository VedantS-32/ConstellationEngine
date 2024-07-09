#pragma once

#include "Core.h"

#include "Window.h"
#include "CStell/Core/LayerStack.h"
#include "CStell/Events/Event.h"
#include "CStell/Events/ApplicationEvent.h"

#include "CStell/Core/Timestep.h"

#include "CStell/ImGui/ImGuiLayer.h"

namespace CStell
{
	
	class Application
	{
	public:
		CSTELL_API Application(const std::string& name = "Constellation Engine");
		CSTELL_API virtual ~Application();

		CSTELL_API void Run();

		CSTELL_API void OnEvent(Event& e);

		CSTELL_API void PushLayer(Layer* layer);
		CSTELL_API void PushOverlay(Layer* layer);

		CSTELL_API  Window& GetWindow() { return *m_Window; }

		CSTELL_API void Close();

		CSTELL_API ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		CSTELL_API static Application& Get();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool m_Running = true;
		bool m_Minimized = false;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};
	
	// To be defined in CLIENT
	CSTELL_API Application* CreateApplication();
}