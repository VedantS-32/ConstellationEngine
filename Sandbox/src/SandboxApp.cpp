#include <CStell.h>
#include <CStellpch.h>

class ExampleLayer : public CStell::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (CStell::Input::IsKeyPressed(CSTELL_KEY_TAB))
			CSTELL_TRACE("Tab key is pressed");
	}

	void OnEvent(CStell::Event& event) override
	{
		CSTELL_TRACE("{0}", event);
	}
};

class Sandbox : public CStell::Application
{

public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new CStell::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};


CStell::Application* CStell::CreateApplication()
{
	CSTELL_INFO("Creating CStell Application!");
	return new Sandbox();
}