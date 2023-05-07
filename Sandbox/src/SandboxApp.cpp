#include <Cstell.h>

class Sandbox : public Cstell::Application {

public:
	Sandbox() {

	}

	~Sandbox() {

	}

};


Cstell::Application* Cstell::CreateApplication() {

	return new Sandbox();
}