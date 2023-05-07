#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern Cstell::Application* Cstell::CreateApplication();

int main(int argc, char** argv) {

	printf("++Constellation Engine++\n");
	Cstell::Log::Init();
	CSTELL_CORE_WARN("Intialized Log!");
	int a = 0;
	CSTELL_INFO("Hello ", a);

	auto app = Cstell::CreateApplication();
	app->Run();
	delete app;
}

#endif
