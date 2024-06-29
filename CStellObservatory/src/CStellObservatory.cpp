#include <CStellpch.h>
#include <CStell.h>
#include <CStell/Core/EntryPoint.h>

#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace CStell
{
	extern RendererAPI* s_RendererAPI;

	class CStellObservatory : public Application
	{
	public:
		CStellObservatory()
			: Application("CStell Observatory")
		{
			ImGui::SetCurrentContext(&Application::Get().GetImGuiLayer()->GetImGuiContext());
			PushLayer(new EditorLayer);
		}

		~CStellObservatory()
		{
		}
	};


	Application* CreateApplication()
	{
		CSTELL_INFO("Creating CStell Application!");
		return new CStellObservatory();
	}
}