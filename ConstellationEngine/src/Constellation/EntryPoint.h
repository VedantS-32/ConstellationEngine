#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern Constellation::Application* Constellation::CreateApplication();

int main(int argc, char** argv) {

	printf("Constellation Engine");
	Cstell::Log::Init();
	CSTELL_CORE_WARN("Intialized Log!");
	int a = 0;
	CSTELL_INFO("Hello ", a);

	auto app = Constellation::CreateApplication();
	app->Run();
	delete app;
}

#endif
