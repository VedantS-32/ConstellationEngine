#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern CStell::Application* CStell::CreateApplication();

int main(int argc, char** argv)
{

	CStell::Log::Init();
	CSTELL_CORE_INFO("Constellation Engine");
	CSTELL_CORE_WARN("Intialized Log!");
	int a = 23;
	CSTELL_INFO("Hello {0}", a);

	auto app = CStell::CreateApplication();
	app->Run();
	delete app;
}

#endif
