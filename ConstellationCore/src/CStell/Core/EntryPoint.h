#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern CStell::Application* CStell::CreateApplication();

int main(int argc, char** argv)
{

	CStell::Log::Init();
	CSTELL_CORE_INFO("Constellation Engine");
	CSTELL_CORE_WARN("Intialized Log!");

	auto app = CStell::CreateApplication();
	app->Run();
	delete app;
}

#endif
