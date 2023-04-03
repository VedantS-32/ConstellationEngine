#include <Cstell.h>

class Sandbox : public Constellation::Application {

public:
	Sandbox() {

	}

	~Sandbox() {

	}

};


Constellation::Application* Constellation::CreateApplication() {

	return new Sandbox();
}