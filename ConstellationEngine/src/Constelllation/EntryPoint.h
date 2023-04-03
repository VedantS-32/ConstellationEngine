#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS

extern Constellation::Application* Constellation::CreateApplication();

int main(int argc, char** argv) {

	//printf("Constellation Engine");
	auto app = Constellation::CreateApplication();
	app->Run();
	delete app;
}

#endif
