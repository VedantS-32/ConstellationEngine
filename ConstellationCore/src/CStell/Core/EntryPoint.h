#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern CStell::Application* CStell::CreateApplication();

int main(int argc, char** argv)
{

	CStell::Log::Init();
	CSTELL_CORE_INFO("Constellation Engine");

	CSTELL_PROFILE_BEGIN_SESSION("Startup", "CStellProfile-Startup.json");
	auto app = CStell::CreateApplication();
	CSTELL_PROFILE_END_SESSION();

	CSTELL_PROFILE_BEGIN_SESSION("Runtime", "CStellProfile-Runtime.json");
	app->Run();
	CSTELL_PROFILE_END_SESSION();

	CSTELL_PROFILE_BEGIN_SESSION("Shutdown", "CStellProfile-Shutdown.json");
	delete app;
	CSTELL_PROFILE_END_SESSION();
}

#endif
