#include "CStellpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "CStell/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ImGuizmo.h>

namespace CStell
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		CSTELL_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		io.Fonts->AddFontFromFileTTF("asset/font/Karla/Static/Karla-Bold.ttf", 17.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("asset/font/Karla/Static/Karla-Regular.ttf", 17.0f);

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// CStellObservatory Themes
		SetDarkThemeColors();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		CSTELL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(); 
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		CSTELL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		CSTELL_PROFILE_FUNCTION();
	}

	void ImGuiLayer::End()
	{
		CSTELL_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		// From https://github.com/Patitotective/ImThemes
		// Fork of Moonlight style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 0.86f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.WindowRounding = 11.5f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(10.0f, 1.0f);
		style.FrameRounding = 5.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(3.0f, 3.0f);
		style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
		style.CellPadding = ImVec2(12.0f, 8.0f);
		style.IndentSpacing = 2.0f;
		style.ColumnsMinSpacing = 5.0f;
		style.ScrollbarSize = 19.5f;
		style.ScrollbarRounding = 2.0f;
		style.GrabMinSize = 8.0f;
		style.GrabRounding = 20.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		// Fork of Purple Comfy style from ImThemes
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3605149984359741f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1235888376832008f, 0.1235888376832008f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1235888376832008f, 0.1235888376832008f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.301960825920105f, 0.535639226436615f, 0.6566523313522339f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.42f, 0.57f, 0.55f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.8755365014076233f, 0.4960120916366577f, 0.6132901906967163f, 0.540772557258606f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1431372612714767f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1549497991800308f, 0.1549497991800308f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2588235437870026f, 0.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 0.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.23f, 0.23f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.3254902064800262f, 0.4901960790157318f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8823529481887817f, 0.3411764800548553f, 0.4509803950786591f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.3411764800548553f, 0.501960813999176f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.9184549450874329f, 0.3390005528926849f, 0.4732949137687683f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.6f, 0.78f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.3529411852359772f, 0.5764706134796143f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.4313725531101227f, 0.250980406999588f, 0.3333333432674408f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9f, 0.34f, 0.47f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.301960825920105f, 0.4817133545875549f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 0.3019607961177826f, 0.4627451002597809f, 0.5333333611488342f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.3f, 0.46f, 0.53f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.3019607961177826f, 0.4627451002597809f, 0.5333333611488342f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9527897238731384f, 0.3884765207767487f, 0.5773882269859314f, 0.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.3f, 0.42f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.301960825920105f, 0.4637383818626404f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.4313725531101227f, 0.250980406999588f, 0.3333333432674408f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.89f, 0.34f, 0.47f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.3019607961177826f, 0.4823529422283173f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509803950786591f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 0.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.88f, 0.46f, 0.6f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.5172318816184998f, 0.7100948095321655f, 0.7725322246551514f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.73f, 0.69f, 0.88f, 0.54f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 0.301960825920105f, 0.4996886253356934f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 0.301960825920105f, 0.445763111114502f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433477878570557f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.301960825920105f, 0.4637383818626404f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
	}
}
